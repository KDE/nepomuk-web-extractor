/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#include <KDebug>
#include <QList>
#include <QString>
#include <QHash>
#include <QUrl>
#include <QTime>

#include "changelog.h"

#include "decision.h"
#include "decisionapplicationrequest.h"
#include "global.h"

namespace NW = Nepomuk::WebExtractor;
namespace NS = Nepomuk::Sync;

class NW::Decision::Private : public QSharedData
{
    public:
	Private();
	// Rank of the Decision. Must be approximately 0 <= rank < 1.
	// The exact boundaries are storeg in globals
	double rank;
	// Every meaningfull Decision consist of one or more 
	// PropertiesGroup. The actuall changes are stored inside
	// PropertiesGroup as NS::ChangeLog object 
	QList< PropertiesGroup > groups;
	// The human-readable description of the Decision
	QString description;

	// There are 2 parts that are necessary for identification.
	// 1) The identification sets for all target resources
	// 2) The identification sets for all resources that are involved
	// into changelog except target ones
	// The first identification sets of the  are taken from the
	// original model. Because of this  the key in the resourceProxyISMap
	// is the url of the resource in the original model
	// The second identification sets are taken from the decisions model.
	// Because of this statements in auxiliaryIdentificationSet use
	// the uri from the decisions model.
	// We need to store the decisions model uri <-> original model uri
	// to correctly identify both target resources and changelog resources
	
	// key is SOURCE url ( as in resourceProxyISMap ), value is 
	// PROXY url ( as in auxiliaryIdentificationSet )
	QHash< QUrl, QUrl > resourceProxyMap;

	// This is our storage for all IdentificationSets for all our
	// proxied resources. The key is the PROXY url, not the source one 
	QHash< QUrl, NS::IdentificationSet > resourceProxyISMap;

	// This is set for all other resources involved 
	// into changes.
	// othersIdentificationSet + all sets from resourceProxyISMap
	// well result in identificationSet of whole Decision change log
	NS::IdentificationSet auxiliaryIdentificationSet;

	// time stamp. The time when the creation of this decision started
	QTime timeStamp;

	// hash name:version of authors datapp
	QHash<QString, int>  authorsData;

	mutable NS::ChangeLog cachedLog;

	mutable bool cachedValidness;
	mutable bool cachedEmptyness;

	mutable QSet<QUrl> cachedTargetResources;

	// dirty bits
	enum { DIRTY_LOG = 1, DIRTY_SETS = 2, DIRTY_VALIDNESS = 4, DIRTY_TARGETRESOURCES = 8 , DIRTY_EMPTYNESS = 16};
	// It is mutable because we need to reset
	// flags in const function
	mutable unsigned char dirty;

};

NW::Decision::Private::Private():
    rank(-1),
    cachedValidness(false),
    cachedEmptyness(false),
    dirty(0xFF)
{;}

NW::Decision::Decision():
    d(new Private())
{;}

NW::Decision::Decision(const Decision & rhs):
    d(rhs.d)
{;}

const NW::Decision & NW::Decision::operator=( const Decision & rhs)
{
    d = rhs.d;
    return *this;
}

NW::Decision::~Decision()
{;}

/*! \brief Return all groups of the Decision
	 */
QList< NW::PropertiesGroup > NW::Decision::groups() const
{
    return d->groups;
}

int NW::Decision::size() const
{
    return d->groups.size();
}

const NW::PropertiesGroup & NW::Decision::group(int index) const
{
    return d->groups[index];
}

bool NW::Decision::isEmpty() const
{
    if ( isDirtyEmptyness() ) {
	kDebug() << "Emptyness is dirty. Rechecking";
	const_cast<NW::Decision*>(this)->d.detach();
	// Check that there is at least any group that is
	// non-empty 
	bool hasNonempty = false;
	foreach( const PropertiesGroup & grp, d->groups )
	{
	    if (!grp.isEmpty()) {
		hasNonempty = true;
		break;
	    }
	}

	d->cachedEmptyness = !hasNonempty;
	
	markCleanEmptyness();
    }
    return d->cachedEmptyness;
}

bool NW::Decision::isValid() const
{
    if (isDirty()) {
	kDebug() << "State is dirty. Rechecking";

	const_cast<NW::Decision*>(this)->d.detach();
	if ( !d->groups.size() ) {
	    kDebug() << "Decision doesn't hold any part";
	    d->cachedValidness = false;
	}
	else {
	    // This variable hold true if in all groups all resources has 
	    // any record in idetification set
	    bool allGroupsAreGood = true;
	    // This is target resoruces expressed in PROXY url terms.
	    QSet<QUrl> tR = targetResources();

	    if (!isEmpty() ) {
		// Check that every non-target resources has an record in
		// identification set
		foreach( const PropertiesGroup & grp, d->groups )
		{
		    // check resources of every group
		    foreach( const QUrl & url, grp.log().resources() )
		    {
			// If target resource doesn't have an identification set,
			// then we have a big big problem
			
			if ( !tR.contains(url) ) { 
			    // It is not a target resource.
			    // May be it is a auxiliary resources ?
			    if ( !d->auxiliaryIdentificationSet.contains(url) )   {
				kDebug() << 
				    "Decision hold some invalid part.\
				    Identification set is uncomplete.\
				    Resource: " << url << "is nor target resource\
				    neither auxiliary resource"
				    ;
				allGroupsAreGood = false;
				break;
			    }
			}

		    }
		    if (!allGroupsAreGood)
			break;
		}
	    }

	    if ( allGroupsAreGood) {
		d->cachedValidness = true;
	    }
	    else {
		d->cachedValidness = false;
	    }

	    // TODO Add check that all non-main resources 
	    // contains records in the auxilaryIdentificationSet
	    // They should conatin at least record with rdf::type
	}

	markCleanValidness();
    }

    return d->cachedValidness;
}


const QHash< QUrl, NS::IdentificationSet> & NW::Decision::identificationSets() const
{
    return d->resourceProxyISMap; 
}

NS::ChangeLog NW::Decision::log() const
{
    if ( isDirtyLog() ) {
	kDebug() << "Log is dirty. Rebuilding";
	const_cast<NW::Decision*>(this)->d.detach();
	NS::ChangeLog answer;
	// update log
	foreach( const PropertiesGroup & grp, d->groups )
	{
	    answer << grp.log();
	}
	d->cachedLog = answer;
	//Q_ASSERT( (answer.size() > 0)?!isEmpty():true );
	markCleanLog();
    }
    return d->cachedLog;
}

NW::DecisionApplicationRequest * NW::Decision::applicationRequest(Soprano::Model * targetModel) const
{
    if( !targetModel)
	targetModel = ResourceManager::instance()->mainModel();
    if(!isValid())
        return 0;


    return new DecisionApplicationRequest(*this, targetModel);
}


bool NW::Decision::apply(Soprano::Model * targetModel) const
{

    if( !targetModel)
	targetModel = ResourceManager::instance()->mainModel();

    if(!isValid())
        return false;

    DecisionApplicationRequest * req = this->applicationRequest(targetModel);

    bool answer = req->apply();
    delete req;
    return answer;

}


double NW::Decision::rank() const
{
    return d->rank;
}

int  NW::Decision::dataPPVersion() const
{
    if (d->authorsData.isEmpty())
	return -1;

    return d->authorsData.begin().value();
}


QString  NW::Decision::dataPPName() const
{
    if (d->authorsData.isEmpty())
	return QString();

    return d->authorsData.begin().key();
}


QString NW::Decision::description() const
{
    return d->description;
}


/* ==== Editing section ==== */
int NW::Decision::addGroup()
{
    // Create group
    d->groups.append(  PropertiesGroup() );

    // Mark as dirty
    markDirtyLog();

    return d->groups.size() - 1;

}

int  NW::Decision::addGroup(const Nepomuk::Sync::ChangeLog & log, const QString & description, double rank )
{
    // Create group
    d->groups.append(  PropertiesGroup(log,description,rank) );

    // Mark as dirty
    markDirtyLog();

    return d->groups.size() - 1;

}

int  NW::Decision::addGroup(const PropertiesGroup & group)
{
    // Create group
    d->groups.append(  group );

    // Mark as dirty
    markDirtyLog();

    return d->groups.size() - 1;

}
void NW::Decision::setDescription( const QString & description)
{
    d->description = description;
}

void NW::Decision::setIdentificationSets( const QHash<QUrl,Nepomuk::Sync::IdentificationSet> isets )
{
    d->resourceProxyISMap = isets;
    markDirtySets();
}


/*
QHash< QUrl, NS::IdentificationSet> & NW::Decision::resourceProxyISMap() 
{
    return d->resourceProxyISMap;
}

QHash< QUrl, NS::IdentificationSet> & NW::Decision::identificationSets()
{
    markDirtySets();
    return d->resourceProxyISMap;
}
*/

void NW::Decision::addIdentificationSet(const QUrl & url, const Nepomuk::Sync::IdentificationSet & iset)
{
    d->resourceProxyISMap[url] = iset;
    markDirtySets();
    return;
}

void NW::Decision::setAuxiliaryIdentificationSet( const Nepomuk::Sync::IdentificationSet & oset)
{
    d->auxiliaryIdentificationSet = oset;
}

NS::IdentificationSet NW::Decision::auxiliaryIdentificationSet() const
{
    return d->auxiliaryIdentificationSet;
}

QSet<QUrl> NW::Decision::targetResources() const
{
    if ( isDirtyTargetResources() ) {
	// Because we edit mutable member
	const_cast<NW::Decision*>(this)->d.detach();
	// Update target resources cache
	// Target resources are all resocures that contains in
	// 1) resourceProxyISMap
	// 2) somewhere in the summary changelog as subjects or objects
	d->cachedTargetResources.clear();
	NS::ChangeLog log = this->log();
	QSet<QUrl> resources = log.resources();
	for( 
		QHash<QUrl,NS::IdentificationSet>::const_iterator it = d->resourceProxyISMap.begin();
		it != d->resourceProxyISMap.end();
		it++
	   )
	{
	    // Check if there is a proxyResource
	    QUrl proxyUrl;

	    QHash<QUrl,QUrl>::const_iterator  fit = d->resourceProxyMap.find(it.key());
	    if ( fit == d->resourceProxyMap.end() ) { 
		// Then proxyUrl is the same as sourceUrl
		proxyUrl = it.key();
	    }
	    else {
		proxyUrl = fit.value();
	    }

	    // Resource is it.key()
	    // check that it contains somewhere in log
	    if (resources.contains(proxyUrl) ) {
		d->cachedTargetResources << proxyUrl;
	    }
	}
	markCleanTargetResources();
    }

    return d->cachedTargetResources;
}

void NW::Decision::cleanUnused()
{
    // Remove empty PropertiesGroup
    QList<PropertiesGroup>::iterator it = d->groups.begin();
    QList<PropertiesGroup>::iterator it_end = d->groups.end();
    for(; it != it_end; it++ )
    {
	if (it->isEmpty()) { // Remove it
	    it = d->groups.erase(it);
	}
    }

    /*
    // Remove unnecessary records from identification hash
    QHash<QUrl,NS::IdentificationSet>::iterator hit = d->resourceProxyISMap.begin();
    QHash<QUrl,NS::IdentificationSet>::iterator hit_end = d->resourceProxyISMap.end();
    QSet<QUrl> tr = targetResources();

    for( ; hit != hit_end; )
    {
	if ( !tr.contains(hit.key()) ) {
	    // Remove from hash
	    hit = d->resourceProxyISMap.erase(hit);
	}
	else {
	    hit++;
	}
    }
    Q_ASSERT(tr.size() == d->resourceProxyISMap.size());
    */

}
/*
void NW::Decision::setMainResources(const QSet<QUrl> & resources )
{
    d->mainResources = resources;
}

void NW::Decision::addMainResources( const QUrl & resource)
{
    d->mainResources.insert(resource);
}
*/

void NW::Decision::setTimeStamp( const QTime & time )
{
    d->timeStamp = time;
}

void NW::Decision::setRank( double rank )
{
    d->rank = boundRank(rank);
}

void NW::Decision::setResourceProxyMap( const QHash<QUrl,QUrl> & map )
{
    d->resourceProxyMap = map;
}

QHash<QUrl,QUrl> NW::Decision::resourceProxyMap() const
{
    return d->resourceProxyMap;
}

void NW::Decision::markDirtyLog()
{
    d->dirty |= Private::DIRTY_LOG;
    d->dirty |= Private::DIRTY_VALIDNESS;
    d->dirty |= Private::DIRTY_TARGETRESOURCES;
    d->dirty |= Private::DIRTY_EMPTYNESS;
}

void NW::Decision::markCleanLog() const
{
    const_cast<NW::Decision*>(this)->d.detach();
    d->dirty = (d->dirty |  Private::DIRTY_LOG ) ^ Private::DIRTY_LOG;
}

void NW::Decision::markDirtySets()
{
    d->dirty |= Private::DIRTY_SETS;
    d->dirty |= Private::DIRTY_VALIDNESS;
    d->dirty |= Private::DIRTY_EMPTYNESS;
    d->dirty |= Private::DIRTY_TARGETRESOURCES;
}

void NW::Decision::markCleanSets() const
{
    const_cast<NW::Decision*>(this)->d.detach();
    d->dirty = (d->dirty |  Private::DIRTY_SETS ) ^ Private::DIRTY_SETS;
}

void NW::Decision::markCleanValidness() const
{
    const_cast<NW::Decision*>(this)->d.detach();
    d->dirty = (d->dirty |  Private::DIRTY_VALIDNESS ) ^ Private::DIRTY_VALIDNESS;
}

void NW::Decision::markCleanEmptyness() const
{
    const_cast<NW::Decision*>(this)->d.detach();
    d->dirty = (d->dirty |  Private::DIRTY_EMPTYNESS ) ^ Private::DIRTY_EMPTYNESS;
}

void NW::Decision::markCleanTargetResources() const
{
    const_cast<NW::Decision*>(this)->d.detach();
    d->dirty = (d->dirty |  Private::DIRTY_TARGETRESOURCES ) ^ Private::DIRTY_TARGETRESOURCES;
}

bool NW::Decision::isDirtyLog() const
{
    return d->dirty & Private::DIRTY_LOG;
}

bool NW::Decision::isDirtySets() const
{
    return d->dirty & Private::DIRTY_SETS;
}

bool NW::Decision::isDirtyTargetResources() const
{
    return d->dirty & Private::DIRTY_TARGETRESOURCES;
}

bool NW::Decision::isDirtyEmptyness() const
{
    return d->dirty & Private::DIRTY_EMPTYNESS;
}

bool NW::Decision::isDirty() const
{
    return d > 0;
}

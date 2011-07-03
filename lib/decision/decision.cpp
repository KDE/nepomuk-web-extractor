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
#include "identificationset.h"

#include "decision.h"
#include "decisionapplicationrequest.h"
#include "global.h"

namespace ND = Nepomuk::Decision;
namespace NS = Nepomuk::Sync;

class ND::Decision::Private : public QSharedData
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

ND::Decision::Private::Private():
    rank(-1),
    cachedValidness(false),
    cachedEmptyness(false),
    dirty(0xFF)
{;}

ND::Decision::Decision():
    d(new Private())
{;}

ND::Decision::Decision(const Decision & rhs):
    d(rhs.d)
{;}

const ND::Decision & ND::Decision::operator=( const Decision & rhs)
{
    d = rhs.d;
    return *this;
}

ND::Decision::~Decision()
{;}

/*! \brief Return all groups of the Decision
	 */
QList< ND::PropertiesGroup > ND::Decision::groups() const
{
    return d->groups;
}

int ND::Decision::size() const
{
    return d->groups.size();
}

const ND::PropertiesGroup & ND::Decision::group(int index) const
{
    return d->groups[index];
}

bool ND::Decision::isEmpty() const
{
    if ( isDirtyEmptyness() ) {
	kDebug() << "Emptyness is dirty. Rechecking";
	const_cast<ND::Decision*>(this)->d.detach();
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

bool ND::Decision::isValid() const
{
    if (isDirty()) {
	kDebug() << "State is dirty. Rechecking";

	const_cast<ND::Decision*>(this)->d.detach();
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


const QHash< QUrl, NS::IdentificationSet> & ND::Decision::identificationSets() const
{
    return d->resourceProxyISMap; 
}

NS::ChangeLog ND::Decision::log() const
{
    if ( isDirtyLog() ) {
	kDebug() << "Log is dirty. Rebuilding";
	const_cast<ND::Decision*>(this)->d.detach();
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

ND::DecisionMetadata ND::Decision::metadata() const
{
    DecisionMetadata answer;
    answer.description = d->description;
    answer.rank = d->rank;
    answer.authorsData = d->authorsData;

    return answer;
}

ND::DecisionApplicationRequest * ND::Decision::applicationRequest(Soprano::Model * targetModel) const
{
    if( !targetModel)
	targetModel = ResourceManager::instance()->mainModel();
    if(!isValid())
        return 0;


    return new DecisionApplicationRequest(*this, targetModel);
}


bool ND::Decision::apply(Soprano::Model * targetModel) const
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


double ND::Decision::rank() const
{
    return d->rank;
}

int  ND::Decision::dataPPVersion() const
{
    if (d->authorsData.isEmpty())
	return -1;

    return d->authorsData.begin().value();
}


QString  ND::Decision::dataPPName() const
{
    if (d->authorsData.isEmpty())
	return QString();

    return d->authorsData.begin().key();
}


QString ND::Decision::description() const
{
    return d->description;
}

/* ==== Storage section ==== */

void ND::Decision::save( QIODevice * device )
{
    QDataStream stream(device);
    this->save(stream);
}

ND::Decision ND::Decision::load( QIODevice * device )
{
    QDataStream stream(device);
    return load(stream);
}

void ND::Decision::save( QDataStream & stream)
{
    /*
    stream << rank << description << resourceProxyMap <<
       resourceProxyISMap << auxilaryIdentificationSet <<
      timeStamp << authorsData << 
      groups;
      */

}


ND::Decision ND::Decision::load( QDataStream & stream )
{
    qDebug() << "Not implemented";
    return Decision();
}

/* ==== Editing section ==== */
int ND::Decision::addGroup()
{
    // Create group
    d->groups.append(  PropertiesGroup() );

    // Mark as dirty
    markDirtyLog();

    return d->groups.size() - 1;

}

int  ND::Decision::addGroup(const Nepomuk::Sync::ChangeLog & log, const QString & description, double rank )
{
    // Create group
    d->groups.append(  PropertiesGroup(log,description,rank) );

    // Mark as dirty
    markDirtyLog();

    return d->groups.size() - 1;

}

int  ND::Decision::addGroup(const PropertiesGroup & group)
{
    // Create group
    d->groups.append(  group );

    // Mark as dirty
    markDirtyLog();

    return d->groups.size() - 1;

}
void ND::Decision::setDescription( const QString & description)
{
    d->description = description;
}

void ND::Decision::setIdentificationSets( const QHash<QUrl,Nepomuk::Sync::IdentificationSet> isets )
{
    d->resourceProxyISMap = isets;
    markDirtySets();
}


/*
QHash< QUrl, NS::IdentificationSet> & ND::Decision::resourceProxyISMap() 
{
    return d->resourceProxyISMap;
}

QHash< QUrl, NS::IdentificationSet> & ND::Decision::identificationSets()
{
    markDirtySets();
    return d->resourceProxyISMap;
}
*/

void ND::Decision::addIdentificationSet(const QUrl & url, const Nepomuk::Sync::IdentificationSet & iset)
{
    d->resourceProxyISMap[url] = iset;
    markDirtySets();
    return;
}

void ND::Decision::setAuxiliaryIdentificationSet( const Nepomuk::Sync::IdentificationSet & oset)
{
    d->auxiliaryIdentificationSet = oset;
}

NS::IdentificationSet ND::Decision::auxiliaryIdentificationSet() const
{
    return d->auxiliaryIdentificationSet;
}

QSet<QUrl> ND::Decision::targetResources() const
{
    if ( isDirtyTargetResources() ) {
	// Because we edit mutable member
	const_cast<ND::Decision*>(this)->d.detach();
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

void ND::Decision::cleanUnused()
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
void ND::Decision::setMainResources(const QSet<QUrl> & resources )
{
    d->mainResources = resources;
}

void ND::Decision::addMainResources( const QUrl & resource)
{
    d->mainResources.insert(resource);
}
*/

void ND::Decision::setTimeStamp( const QTime & time )
{
    d->timeStamp = time;
}

void ND::Decision::setRank( double rank )
{
    d->rank = boundRank(rank);
}

void ND::Decision::setResourceProxyMap( const QHash<QUrl,QUrl> & map )
{
    d->resourceProxyMap = map;
}

QHash<QUrl,QUrl> ND::Decision::resourceProxyMap() const
{
    return d->resourceProxyMap;
}

void ND::Decision::markDirtyLog()
{
    d->dirty |= Private::DIRTY_LOG;
    d->dirty |= Private::DIRTY_VALIDNESS;
    d->dirty |= Private::DIRTY_TARGETRESOURCES;
    d->dirty |= Private::DIRTY_EMPTYNESS;
}

void ND::Decision::markCleanLog() const
{
    const_cast<ND::Decision*>(this)->d.detach();
    d->dirty = (d->dirty |  Private::DIRTY_LOG ) ^ Private::DIRTY_LOG;
}

void ND::Decision::markDirtySets()
{
    d->dirty |= Private::DIRTY_SETS;
    d->dirty |= Private::DIRTY_VALIDNESS;
    d->dirty |= Private::DIRTY_EMPTYNESS;
    d->dirty |= Private::DIRTY_TARGETRESOURCES;
}

void ND::Decision::markCleanSets() const
{
    const_cast<ND::Decision*>(this)->d.detach();
    d->dirty = (d->dirty |  Private::DIRTY_SETS ) ^ Private::DIRTY_SETS;
}

void ND::Decision::markCleanValidness() const
{
    const_cast<ND::Decision*>(this)->d.detach();
    d->dirty = (d->dirty |  Private::DIRTY_VALIDNESS ) ^ Private::DIRTY_VALIDNESS;
}

void ND::Decision::markCleanEmptyness() const
{
    const_cast<ND::Decision*>(this)->d.detach();
    d->dirty = (d->dirty |  Private::DIRTY_EMPTYNESS ) ^ Private::DIRTY_EMPTYNESS;
}

void ND::Decision::markCleanTargetResources() const
{
    const_cast<ND::Decision*>(this)->d.detach();
    d->dirty = (d->dirty |  Private::DIRTY_TARGETRESOURCES ) ^ Private::DIRTY_TARGETRESOURCES;
}

bool ND::Decision::isDirtyLog() const
{
    return d->dirty & Private::DIRTY_LOG;
}

bool ND::Decision::isDirtySets() const
{
    return d->dirty & Private::DIRTY_SETS;
}

bool ND::Decision::isDirtyTargetResources() const
{
    return d->dirty & Private::DIRTY_TARGETRESOURCES;
}

bool ND::Decision::isDirtyEmptyness() const
{
    return d->dirty & Private::DIRTY_EMPTYNESS;
}

bool ND::Decision::isDirty() const
{
    return d > 0;
}

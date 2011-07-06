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
#include <KJob>


#include "decision.h"
#include "global.h"

#include <nepomuk/datamanagement.h>


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


	// time stamp. The time when the creation of this decision started
	QTime timeStamp;

	// hash name:version of authors datapp
	QHash<QString, int>  authorsData;

        QSet<QUrl> targetResources;

	//mutable NS::ChangeLog cachedLog;

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
        d->cachedValidness = true;
	markCleanValidness();
    }

    return d->cachedValidness;
}


#if 0
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
#endif

ND::DecisionMetadata ND::Decision::metadata() const
{
    DecisionMetadata answer;
    answer.description = d->description;
    answer.rank = d->rank;
    answer.authorsData = d->authorsData;

    return answer;
}

#if 0
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
#endif

KJob * ND::Decision::applyJob() const
{
    // Unite all changes into one
    SimpleResourceGraph result_changes;
    foreach( const PropertiesGroup & pg, this->d->groups)
    {
        result_changes.addGraph(pg.changes());
    }
    KJob * answer = storeResources(result_changes,Nepomuk::IdentifyNew);

    return answer;
}

bool ND::Decision::apply() const
{
    KJob * job = applyJob();
    if (!job->exec() ) {
        qDebug() << "Can't apply Decision";
        return false;
    }
    return true;
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

void ND::Decision::save( QIODevice * device ) const
{
    QDataStream stream(device);
    this->save(stream);
}

ND::Decision ND::Decision::load( QIODevice * device ) 
{
    QDataStream stream(device);
    return load(stream);
}

void ND::Decision::save( QDataStream & stream)const
{
    stream << d->rank << d->description << 
      d->timeStamp << d->authorsData << 
      d->groups;

}


ND::Decision ND::Decision::load( QDataStream & stream )
{
    Decision answer;
    stream >> answer.d->rank >> answer.d->description >> 
      answer.d->timeStamp >> answer.d->authorsData >> 
      answer.d->groups;
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

/*
int  ND::Decision::addGroup(const ??? & log, const QString & description, double rank )
{
    // Create group
    d->groups.append(  PropertiesGroup(log,description,rank) );

    // Mark as dirty
    markDirtyLog();

    return d->groups.size() - 1;

}
*/

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


QSet<QUrl> ND::Decision::targetResources() const
{
    return d->targetResources;
}

void ND::Decision::setTargetResources( const QSet<QUrl> & targetResources)
{
    d->targetResources = targetResources;
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


}

void ND::Decision::setTimeStamp( const QTime & time )
{
    d->timeStamp = time;
}

void ND::Decision::setRank( double rank )
{
    d->rank = boundRank(rank);
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

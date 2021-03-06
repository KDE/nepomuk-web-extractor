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

#include "decisionlist.h"
#include <math.h>

/*
Nepomuk::Decision::DecisionList::DecisionList():
{;}
*/

namespace ND = Nepomuk::Decision;

Nepomuk::Decision::DecisionList::DecisionList(double threshold):
    m_threshold(threshold),
    m_trusted(true),
    m_hasAutoApplicable(false)
{
    ;
}


void Nepomuk::Decision::DecisionList::mergeWith(const DecisionList & rhs,  MergePolitics politics, double coff)
{
    if(&rhs == this)
        return;

    /*
    this->append(rhs);
    this->unique(politics, coff);
    */

    for(DecisionList::const_iterator it = rhs.begin(); it != rhs.end(); it++)
        //foreach( Decision d, rhs)
    {
        Decision d = Decision(*it);
        addDecisionUnscaled(d, politics, coff);
    }
}

/*
void Nepomuk::Decision::DecisionList::unique( MergePolitics policis , double coff )
{
    kDebug() << "Not realized yet";
}
*/


/*
void Nepomuk::Decision::DecisionList::scale(double coff)
{
    for(DecisionList::iterator it = begin(); it != end(); it++)
    {
    it->setRank(scaledRank(it->rank(), coff));
    }
}
*/


/*
void Nepomuk::Decision::DecisionList::sort()
{
    kDebug() << "Not realized yet";
}
*/



void Nepomuk::Decision::DecisionList::addDecision(const Decision &  dec, bool noAuto)
{
    addDecision(dec, Highest, 1.0, noAuto);
}

void Nepomuk::Decision::DecisionList::addDecision(const Decision &  dec, MergePolitics politics, double coff, bool noAuto)
{
    if (!dec.isValid()) {
	kDebug() << "Decision is invalid";
	return;
    }
    
    if (dec.isEmpty()) {
	kDebug() << "Decision is empty";
	return;
    }

    Decision d(dec);
    double newRank = scaledRank(dec.rank(), m_scaleCoff);
    // Correct rank if necessary
    if((!noAuto) || (!m_trusted))  {
        // Then this decision must not has auto applicable rank
        if(newRank > m_acrit)
            d.setRank(m_acrit);
    }
    addDecisionUnscaled(d, politics, coff);
}

void Nepomuk::Decision::DecisionList::addDecisionUnscaled(Decision &  d, MergePolitics politics, double coff)
{
    // TODO Disable checkings and rank updating of the Decisions
    // entirely. It should be completely rewritten
#if 0
    double newRank = d.rank();
    // Ignore decision with low rank and empty decisions
    if((newRank > m_threshold) and(!d.isEmpty())) {
        // If such decision already exists
        QList<Decision>::iterator dit = this->find(d);
        if(dit != this->end()) {
            kDebug() << "Decision already exist. Update rank";
            // Adjust it rank acording to politics
            double nnRank = 0;
            double ditRank = dit->rank();
            switch(politics) {
            case Lowest : {
                nnRank = qMin(newRank, ditRank);
                break;
            }
            case Average : {
                nnRank = (newRank + ditRank) / 2.0;
                break;
            }
            case Highest : {
                nnRank = qMax(newRank, ditRank);
                break;
            }
            case Adjust : {
                nnRank = pow(ditRank, 1 - newRank);
                break;
            }
            }

            // Update hasAutoApplicable flag
            if(nnRank > m_acrit)
                m_hasAutoApplicable = true;

            // Update best decision
            if(nnRank > m_best.rank())
                m_best = d;

            d.setRank(nnRank);

            this->erase(dit);

            this->insert(d);

        } else {
            kDebug() << "Add Decision";
            // Add this decision
            d.setRank(newRank);

            // Update hasAutoApplicable flag
            if(newRank > m_acrit)
                m_hasAutoApplicable = true;

            this->insert(d);

            // Update best decision
            if(newRank > m_best.rank())
                m_best = d;
        }
    }
#endif
	// Update hasAutoApplicable flag
	if(d.rank() > m_acrit)
	    m_hasAutoApplicable = true;

	// Update best decision
	if(d.rank() > m_best.rank())
	    m_best = d;

	this->append(d);
}

bool Nepomuk::Decision::DecisionList::hasAutoApplicable() const
{
    return m_hasAutoApplicable;
}


QSet<const Nepomuk::Decision::DecisionAuthor *> Nepomuk::Decision::DecisionList::filterObsolete()
{
    kDebug() << "Not realzed yet";
    return QSet<const DecisionAuthor *>();
}

void Nepomuk::Decision::DecisionList::filter(double threshold)
{
    kDebug() << "Not realzed yet";
}

void Nepomuk::Decision::DecisionList::addToUserDiscretion() const
{
    kDebug() << "Not realzed yet";
}

const Nepomuk::Decision::Decision &  Nepomuk::Decision::DecisionList::best() const
{
    return m_best;
}

double Nepomuk::Decision::DecisionList::scaledRank(double rank, double coff)
{
    if(coff >= 0)
        return pow(rank, 1 - coff);
    else
        return pow(rank, 1.0 / (1 + coff));

}
ND::DecisionList::const_iterator ND::DecisionList::begin() const
{
    return QList<Decision>::begin();
}
ND::DecisionList::const_iterator ND::DecisionList::end() const
{
    return QList<Decision>::end();
}

double ND::DecisionList::approximateThreshold() const
{
    return m_threshold;
}

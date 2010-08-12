#include "decisionlist.h"
#include <math.h>

/*
Nepomuk::WebExtractor::DecisionList::DecisionList():
{;}
*/

namespace NW = Nepomuk::WebExtractor;

Nepomuk::WebExtractor::DecisionList::DecisionList(double threshold):
    m_threshold(threshold),
    m_trusted(true),
    m_hasAutoApplicable(false)
{
    ;
}


void Nepomuk::WebExtractor::DecisionList::mergeWith(const DecisionList & rhs,  WE::MergePolitics politics, double coff)
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
void Nepomuk::WebExtractor::DecisionList::unique( WE::MergePolitics policis , double coff )
{
    kDebug() << "Not realized yet";
}
*/


/*
void Nepomuk::WebExtractor::DecisionList::scale(double coff)
{
    for(DecisionList::iterator it = begin(); it != end(); it++)
    {
    it->setRank(scaledRank(it->rank(), coff));
    }
}
*/


/*
void Nepomuk::WebExtractor::DecisionList::sort()
{
    kDebug() << "Not realized yet";
}
*/



void Nepomuk::WebExtractor::DecisionList::addDecision(const Decision &  dec, bool noAuto)
{
    addDecision(dec, WE::Highest, 1.0, noAuto);
}

void Nepomuk::WebExtractor::DecisionList::addDecision(const Decision &  dec, WE::MergePolitics politics, double coff, bool noAuto)
{
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

void Nepomuk::WebExtractor::DecisionList::addDecisionUnscaled(Decision &  d, WE::MergePolitics politics, double coff)
{
    // TODO We should freeze decisions after they are added to the list
    double newRank = d.rank();
    // Ignore decision with low rank and empty decisions
    if((newRank > m_threshold) and(!d.isEmpty())) {
        // If such decision already exists
        QSet<Decision>::iterator dit = this->find(d);
        if(dit != this->end()) {
            kDebug() << "Decision already exist. Update rank";
            // Adjust it rank acording to politics
            double nnRank = 0;
            double ditRank = dit->rank();
            switch(politics) {
            case WE::Lowest : {
                nnRank = qMin(newRank, ditRank);
                break;
            }
            case WE::Average : {
                nnRank = (newRank + ditRank) / 2.0;
                break;
            }
            case WE::Highest : {
                nnRank = qMax(newRank, ditRank);
                break;
            }
            case WE::Adjust : {
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
    //kDebug() << "Not realized yet";
}

bool Nepomuk::WebExtractor::DecisionList::hasAutoApplicable() const
{
    //kDebug() << "Not realized yet";
    return m_hasAutoApplicable;
}


QSet<const Nepomuk::WebExtractor::DataPP *> Nepomuk::WebExtractor::DecisionList::filterObsolete()
{
    kDebug() << "Not realzed yet";
    return QSet<const DataPP *>();
}

void Nepomuk::WebExtractor::DecisionList::filter(double threshold)
{
    kDebug() << "Not realzed yet";
}

void Nepomuk::WebExtractor::DecisionList::addToUserDiscretion() const
{
    kDebug() << "Not realzed yet";
}

const Nepomuk::WebExtractor::Decision &  Nepomuk::WebExtractor::DecisionList::best() const
{
    return m_best;
}

double Nepomuk::WebExtractor::DecisionList::scaledRank(double rank, double coff)
{
    if(coff >= 0)
        return pow(rank, 1 - coff);
    else
        return pow(rank, 1.0 / (1 + coff));

}
NW::DecisionList::const_iterator NW::DecisionList::begin() const
{
    return QSet<Decision>::begin();
}
NW::DecisionList::const_iterator NW::DecisionList::end() const
{
    return QSet<Decision>::end();
}
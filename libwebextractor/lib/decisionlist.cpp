#include <webextractor/decisionlist.h>

/*
Nepomuk::WebExtractor::DecisionList::DecisionList():
{;}
*/

Nepomuk::WebExtractor::DecisionList::DecisionList(double threshold ):
    m_threshold(threshold),
    m_trusted(true)
{;}


void Nepomuk::WebExtractor::DecisionList::mergeWith( const DecisionList & rhs, double scale, WE::MergePolitics politics, double coff )
{
    if (&rhs == this)
	return;

    /*
    this->append(rhs);
    this->unique(politics, coff);
    */

    for( DecisionList::const_iterator it = rhs.begin(); it != rhs.end(); it++)
    //foreach( Decision d, rhs)
    {
	Decision d = Decision(*it);
	d.setRank(scale * d.rank() );
	addDecision( d, politics, coff);
    }
}

void Nepomuk::WebExtractor::DecisionList::unique( WE::MergePolitics policis , double coff )
{
    kDebug() << "Not realized yet";
}

void Nepomuk::WebExtractor::DecisionList::scale(double coff)
{
    for(DecisionList::iterator it = begin(); it != end(); it++)
    {
	it->setRank(it->rank()*coff);
    }
}

void Nepomuk::WebExtractor::DecisionList::sort()
{
    kDebug() << "Not realized yet";
}

void Nepomuk::WebExtractor::DecisionList::addDecision( const Decision &  dec, bool noAuto )
{
    addDecision( dec, WE::Highest, 1.0, noAuto );
}

void Nepomuk::WebExtractor::DecisionList::addDecision( const Decision &  dec, WE::MergePolitics politics, double coff, bool noAuto )
{
    Decision d(dec);
    // Correct rank if necessary
    if ( (!noAuto) || (!m_trusted))  {
	// Then this decision must not has auto applicable rank
	if ( d.rank() > m_acrit )
	    d.setRank(m_acrit);
    }
    // Ignore decision with low rank and empty decisions
    if ( ( d.rank() < m_threshold ) and (!d.isEmpty()) ) {
	this->push_back(dec);
    }
    kDebug() << "Not realized yet";
}

bool Nepomuk::WebExtractor::DecisionList::hasAutoApplicable() const
{
    kDebug() << "Not realized yet";
    return false;
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
    static Decision d = Decision(0);
    kDebug() << "Not realzed yet";
    return d;
}

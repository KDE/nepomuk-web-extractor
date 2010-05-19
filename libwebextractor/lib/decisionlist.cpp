#include <webextractor/decisionlist.h>

/*
Nepomuk::WebExtractor::DecisionList::DecisionList():
{;}
*/

Nepomuk::WebExtractor::DecisionList::DecisionList(double threshold ):
    m_threshold(threshold)
{;}


void Nepomuk::WebExtractor::DecisionList::mergeWith( const DecisionList & rhs, double scale, MergePolitics politics, double coff )
{
    if (&rhs == this)
	return;

    /*
    this->append(rhs);
    this->unique(politics, coff);
    */

    Decision d;
    for( DecisionList::const_iterator it = rhs.begin(); it != rhs.end(); it++)
    //foreach( Decision d, rhs)
    {
	d = *it;
	d.setRank(scale * d.rank() );
	addDecision( d, politics, coff);
    }
}

void Nepomuk::WebExtractor::DecisionList::unique( MergePolitics policis , double coff )
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

void Nepomuk::WebExtractor::DecisionList::addDecision( const Decision &  dec )
{
    if ( dec.rank() < m_threshold )
	this->push_back(dec);
}

void Nepomuk::WebExtractor::DecisionList::addDecision( const Decision &  dec, MergePolitics politics, double coff )
{
    if ( dec.rank() >= m_threshold) {
	this->push_back(dec);
    }
    kDebug() << "Not realized yet";
}

bool Nepomuk::WebExtractor::DecisionList::hasAutoApplicable() const
{
    kDebug() << "Not realized yet";
    return false;
}



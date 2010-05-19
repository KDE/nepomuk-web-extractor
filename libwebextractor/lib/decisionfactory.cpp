#include <webextractor/decisionfactory.h>

Nepomuk::WebExtractor::DecisionFactory::DecisionFactory():
    m_threshold(0)
{;}

void Nepomuk::WebExtractor::DecisionFactory::setThreshold(double threshold)
{
    m_threshold = threshold;
}
Nepomuk::WebExtractor::Decision Nepomuk::WebExtractor::DecisionFactory::newDecision()
{
     return Decision();
}

Nepomuk::WebExtractor::DecisionList  Nepomuk::WebExtractor::DecisionFactory::newDecisionList()
{
    Nepomuk::WebExtractor::DecisionList lst;
    lst.m_threshold = m_threshold;
    return lst;
}


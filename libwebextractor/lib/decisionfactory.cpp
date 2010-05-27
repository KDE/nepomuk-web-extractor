#include <webextractor/decisionfactory.h>
//#include <webextractor/datapp.h>
#include <webextractor/global.h>

Nepomuk::WebExtractor::DecisionFactory::DecisionFactory(double ucrit, double acrit):
    m_threshold(0)
{
    Q_ASSERT(acrit == Nepomuk::WebExtractor::WE::boundACrit(acrit));
    Q_ASSERT(ucrit == Nepomuk::WebExtractor::WE::boundUCrit(ucrit));
    m_threshold = ucrit;
    m_acrit = acrit;
}

void Nepomuk::WebExtractor::DecisionFactory::setThreshold(double threshold)
{
    m_threshold = threshold;
}
Nepomuk::WebExtractor::Decision Nepomuk::WebExtractor::DecisionFactory::newDecision(const DataPP * parent) const
{
     return Decision(parent);
}

Nepomuk::WebExtractor::DecisionList  Nepomuk::WebExtractor::DecisionFactory::newDecisionList() const
{
    Nepomuk::WebExtractor::DecisionList lst;
    lst.m_threshold = m_threshold;
    lst.m_acrit = m_acrit;
    return lst;
}


#include "decisionfactory.h"
//#include "datapp.h"
#include "global.h"

namespace NW=Nepomuk::WebExtractor;

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

Nepomuk::WebExtractor::DecisionList  Nepomuk::WebExtractor::DecisionFactory::newDecisionList(const DataPP*) const
{
    Nepomuk::WebExtractor::DecisionList lst;
    lst.m_threshold = m_threshold;
    lst.m_acrit = m_acrit;
    return lst;
}

Nepomuk::WebExtractor::PropertiesGroup Nepomuk::WebExtractor::DecisionFactory::newPropertiesGroup(const DataPP * parent) const
{
     return PropertiesGroup();
}

Nepomuk::WebExtractor::DecisionList  Nepomuk::WebExtractor::DecisionFactory::newDecisionList() const
{
    Nepomuk::WebExtractor::DecisionList lst;
    lst.m_threshold = m_threshold;
    lst.m_acrit = m_acrit;
    return lst;
}

Nepomuk::WebExtractor::DecisionFactory * NW::DecisionFactory::debugFactory(double ucrit, double acrit)
{
    static DecisionFactory * m_factory = new DecisionFactory(ucrit,acrit);
    return m_factory;
}

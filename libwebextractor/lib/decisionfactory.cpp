#include <webextractor/decisionfactory.h>
//#include <webextractor/datapp.h>
#include <webextractor/global.h>

Nepomuk::WebExtractor::DecisionFactory::DecisionFactory(const QString & name, const QString & version, DecisionFactoryBase * base):
    m_name(name),
    m_version(version),
    m_base(base)
{
    Q_CHECK_PTR(m_base);
}

Nepomuk::WebExtractor::Decision Nepomuk::WebExtractor::DecisionFactory::newDecision() const
{
     return m_base->newDecision(m_name,m_version);
}

Nepomuk::WebExtractor::DecisionList  Nepomuk::WebExtractor::DecisionFactory::newDecisionList() const
{
    return m_base->newDecisionList();
}


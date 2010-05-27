#include <webextractor/datappreply.h>
#include <webextractor/datapp.h>

class Nepomuk::WebExtractor::DataPPReply::Private
{
    public:
	QString m_pluginName;
	QString m_pluginVersion;
	DecisionFactory * m_fact;
};

Nepomuk::WebExtractor::DataPPReply::DataPPReply(
//	const QString & pluginName
	//const QString & pluginVersion
	DataPP * parent,
	DecisionFactory * fact;
	)
{
    d = new Private();
    Q_CHECK_PTR(parent);
    Q_CHECK_PTR(fact);
    d->m_pluginVersion = parent->pluginVersion();
    d->m_pluginName = parent->pluginName();
    d->m_fact = fact;
}

void Nepomuk::WebExtractor::DataPPReply::setPluginName(const QString & pluginName)
{
    d->m_pluginName = pluginName;
}

Nepomuk::WebExtractor::DataPPReply::~DataPPReply()
{
    delete d;
}

const QString & Nepomuk::WebExtractor::DataPPReply::pluginName() const 
{ return d->m_pluginName; }

Decison Nepomuk::WebExtractor::DataPPReply::newDecision()
{
    Decison d = m_fact->newDecision(d->m_pluginName, d->m_pluginVersion);
    return d;
}

DecisonList Nepomuk::WebExtractor::DataPPReply::newDecisionList()
{
    return m_fact->newDecisionList();
}

/*
const QString & Nepomuk::WebExtractor::DataPPReply::pluginVersion() const 
{ return d->m_pluginVersion; }

*/

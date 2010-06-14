#include <webextractor/datappreply.h>
#include <webextractor/datapp.h>

class Nepomuk::WebExtractor::DataPPReply::Private
{
    public:
	DataPP * m_parent;
};

Nepomuk::WebExtractor::DataPPReply::DataPPReply(
	DataPP * parent
	)
{
    d = new Private();
    Q_CHECK_PTR(parent);
    d->m_parent = parent;
}

/*
void Nepomuk::WebExtractor::DataPPReply::setPluginName(const QString & pluginName)
{
    d->m_pluginName = pluginName;
}
*/

Nepomuk::WebExtractor::DataPPReply::~DataPPReply()
{
    delete d;
}

Nepomuk::WebExtractor::DataPP * Nepomuk::WebExtractor::DataPPReply::parent() const
{ return d->m_parent;}


const QString & Nepomuk::WebExtractor::DataPPReply::pluginName() const 
{ return d->m_parent->pluginName(); }


/*
const QString & Nepomuk::WebExtractor::DataPPReply::pluginVersion() const 
{ return d->m_pluginVersion; }

*/

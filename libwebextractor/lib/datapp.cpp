#include <webextractor/datapp.h>


class Nepomuk::WebExtractor::DataPP::Private 
{
    public:
	QString m_version;
	QString m_name;
};
Nepomuk::WebExtractor::DataPP::~DataPP()
{;}

Nepomuk::WebExtractor::DataPP::DataPP( const QString & pluginVersion)
{
    d = new Private();
    d->m_version = pluginVersion;
}

void Nepomuk::WebExtractor::DataPP::setPluginName( const QString & name)
{
    d->m_name = name;
}
QString Nepomuk::WebExtractor::DataPP::pluginName() const 
{ return d->m_name;}

QString Nepomuk::WebExtractor::DataPP::pluginVersion() const 
{ return d->m_version;}

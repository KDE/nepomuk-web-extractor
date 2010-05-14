#include "webextractor_settings.h"
#include <QtCore/QString>

Nepomuk::WebExtractor::WebExtractorConfig::WebExtractorConfig():
    WebExConfigBase()
{
    QStringList cats = categories();
    foreach( QString cat, cats)
    {
	if (!m_categories.contains(cat)) {
	    this->m_categories[cat] = new WebExCategory(KSharedConfig::openConfig(cat+"rc"));
	}
    }
}

#if 0
QDebug Nepomuk::WebExtractor::WebExCategory::operator<<( QDebug dbg,  const WebExCategory & cat)
{
    dbg<<cat.queue();
}

QDebug Nepomuk::WebExtractor::WebExtractorConfig::operator<<( QDebug dbg,  const WebExtractorConfig & conf)
{
    QStringList cats = self()->categories();
    dbg<<"Categories:";
    foreach( QString cat, cats)
    {
	dbg<<cat;	
    }
}

#endif

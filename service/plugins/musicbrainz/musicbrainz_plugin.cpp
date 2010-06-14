#include "musicbrainz_datapp.h"
#include "musicbrainzconfig.h"


K_PLUGIN_FACTORY(MusicBrainzWEFactory,
                 registerPlugin<MusicBrainzWEPlugin>();
                )
K_EXPORT_PLUGIN(MusicBrainzWEFactory("musicbrainz"))

Nepomuk::MusicBrainzWEPlugin::MusicBrainzWEPlugin()
{
}

Nepomuk::WebExtractor::DataPP * MusicBrainzWEPlugin::getDataPP( KSharedConfigPtr configFile )
{
    MusicBrainzConfig * c = new MusicBrainzConfig(configFile);
    QString serverName = c->serverName();
    if ( 
	    (!m_datappCache.contains(serverName)) || 
	    (m_datappCache[serverName].isNull()) 
	    ) 
    {

	MusicBrainzDataPP* mdpp =  new MusicBrainzDataPP(serverName);
	m_datappCache[serverName] = QPointer<MusicBrainzDataPP>(mdpp);
	return mdpp;
    }
    else { 
	return m_datappCache[serverName];
    }

}

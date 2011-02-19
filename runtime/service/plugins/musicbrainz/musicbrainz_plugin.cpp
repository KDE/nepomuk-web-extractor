#include "musicbrainz_executive.h"
#include "musicbrainzconfig.h"


K_PLUGIN_FACTORY(MusicBrainzWEFactory,
                 registerPlugin<MusicBrainzWEPlugin>();
                )
K_EXPORT_PLUGIN(MusicBrainzWEFactory("musicbrainz"))

Nepomuk::MusicBrainzWEPlugin::MusicBrainzWEPlugin()
{
}

Nepomuk::WebExtractor::Executive * MusicBrainzWEPlugin::getExecutive( KSharedConfigPtr configFile )
{
    MusicBrainzConfig * c = new MusicBrainzConfig(configFile);
    QString serverName = c->serverName();
    if ( 
	    (!m_datappCache.contains(serverName)) || 
	    (m_datappCache[serverName].isNull()) 
	    ) 
    {

	MusicBrainzExecutive* mdpp =  new MusicBrainzExecutive(serverName);
	m_datappCache[serverName] = QPointer<MusicBrainzExecutive>(mdpp);
	return mdpp;
    }
    else { 
	return m_datappCache[serverName];
    }

}

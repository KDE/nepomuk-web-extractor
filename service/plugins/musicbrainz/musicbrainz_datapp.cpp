#include "musicbrainz_datapp.h"
#include "musicbrainz_config.h"


Nepomuk::MusicBrainzDataPP::MusicBrainzDataPP(const QString & serverName ):
    DataPP(MUSIC_BRAINZ_VERSION)
{
    m_net = new QNetworkAcessManager();
}

Nepomuk::MusicBrainzDataPP::~MusicBrainzDataPP()
{
    delete m_net;
}

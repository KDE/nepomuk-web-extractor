#include "dbpmusicreply.h"

Nepomuk::DbpMusicReply::DbpMusicReply(DbpMusicDataPP * parent, WebExtractor::DecisionFactory * factory, const Nepomuk::Resource & res ):
    SimpleDataPPReply(parent, factory, res)
{
    // The resource have necessary type. This condition was checked in DbpMusicDataPP
    // Create a query
    //
    Soprano::SparqlModel * model = static_cast<DbpMusicDataPP*>(this->parentDataPP())->m_model;

}

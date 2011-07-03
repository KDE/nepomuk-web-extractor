#include "dbpmusicreply.h"

Nepomuk::DbpMusicReply::DbpMusicReply(DbpMusicExecutive * parent, WebExtractor::DecisionFactory * factory, const Nepomuk::Resource & res ):
    SimpleExecutiveReply(parent, factory, res)
{
    // The resource have necessary type. This condition was checked in DbpMusicExecutive
    // Create a query
    //
    Soprano::SparqlModel * model = static_cast<DbpMusicExecutive*>(this->parentExecutive())->m_model;

}

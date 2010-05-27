#include <webextractor/datappproxy.h>

namespace NW=Nepomuk::WebExtractor;

NW::DataPPProxy::DataPPProxy(DataPP * dpp, double rank)
{
    Q_CHECK_PTR(dpp);
    m_data = dpp;
    m_rank = rank;
}

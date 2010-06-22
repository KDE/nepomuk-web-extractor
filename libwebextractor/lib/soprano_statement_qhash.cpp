#include <webextractor/soprano_statement_qhash.h>
#include <QHash>

unsigned int Soprano::qHash( const Soprano::Statement & st)
{
    return qHash(st.object()) ^ qHash(st.predicate()) ^ qHash(st.subject());
}



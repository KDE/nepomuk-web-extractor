#ifndef _sporano_statemenet_qhash_h_
#define _sporano_statemenet_qhash_h_

#include <QtCore/QObject>
#include <Soprano/Statement>

namespace Soprano {
    unsigned int qHash( const Soprano::Statement & );
}

#endif

//#include "webexservice.cpp"
#include <QtCore/QCoreApplication>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtCore/QThread>
#include <QtDBus/QDBusConnection>
#include <Nepomuk/Resource>
#include <webexservice.h>
#include "datapppool.h"
#include <KDebug>
/*
#include <Nepomuk/Query>
#include <Nepomuk/ComparisonTerm>
#include <Soprano/Vocabulary/NAO>
*/

using namespace std;
using namespace Nepomuk;
namespace NQ = Nepomuk::Query;
int main(int argc, char ** argv)
{
    QCoreApplication app(argc, argv);
    /*
    NQ::Query query(NQ::NegationTerm::negateTerm(NQ::ComparisonTerm(
                        Soprano::Vocabulary::NAO::version(),
                        NQ::Term()
                    )));
    kDebug() << query.toSparqlQuery();
    */

    QList<QVariant> noth;
    kDebug() << "Main thread. I am" << QThread::currentThreadId();
    kDebug() << *(DataPPPool::self());
    Nepomuk::WebExtractorService *srv  = new Nepomuk::WebExtractorService(0, noth);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/webexservice", srv);
    dbus.registerService("org.kde.webexserviceexample");
    return app.exec();

}

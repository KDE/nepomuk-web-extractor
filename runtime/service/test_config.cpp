//#include "webexservice.cpp"
#include <QtCore/QCoreApplication>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtCore/QThread>
#include <QtDBus/QDBusConnection>
#include <Nepomuk/Resource>
#include <webexservice.h>
#include "datapppool.h"
#include <webextractor_settings.h>
#include <KDebug>
#include <QtDebug>

using namespace std;
using namespace Nepomuk;

int main(int argc, char ** argv)
{
    QCoreApplication app(argc,argv);
    QList<QVariant> noth;
#if 0
    kDebug() << "Main thread. I am"<< QThread::currentThreadId();
    Nepomuk::WebExtractorService *srv  = new Nepomuk::WebExtractorService(0,noth);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/webexservice", srv);
    dbus.registerService("org.kde.webexserviceexample");
#endif
    //((int __s = QThread::idealThreadCount()) > 0)?__s:1;
    WebExtractorSettings c;
    //Nepomuk::WebExtractor::operator<<( qDebug(),  c);
    kDebug() << c;
    kDebug() << *(DataPPPool::self());
    //return app.exec();

}

//#include "webexservice.cpp"
#include <QtCore/QCoreApplication>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtCore/QThread>
#include <QtDBus/QDBusConnection>
#include <Nepomuk/Resource>
#include <webexservice.h>
#include <KDebug>

using namespace std;
using namespace Nepomuk;
int main(int argc, char ** argv)
{
    QCoreApplication app(argc,argv);
    QList<QVariant> noth;
    kDebug() << "Main thread. I am"<< QThread::currentThreadId();
    Nepomuk::WebExtractorService *srv  = new Nepomuk::WebExtractorService(0,noth);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/webexservice", srv);
    dbus.registerService("org.kde.webexserviceexample");
    return app.exec();

}

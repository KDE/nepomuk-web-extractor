//#include "webexservice.cpp"
#include <QtCore/QCoreApplication>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <Nepomuk/Resource>
#include <webexservice.h>
using namespace std;
using namespace Nepomuk;
int main(int argc, char ** argv)
{
    QCoreApplication app(argc,argv);
    QList<QVariant> noth;
    Nepomuk::WebExtractorService srv(0,noth);
    return app.exec();

}

//#include "webexservice.cpp"
#include <QtCore/QCoreApplication>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <Nepomuk/Resource>
#include <webextractor/resourceanalyzerfactory.h>
using namespace std;
using namespace Nepomuk::WebExtractor;
int main(int argc, char ** argv)
{
#if 0
    QCoreApplication app(argc,argv);
    QList<QVariant> noth;
    ResourceAnalyzerFactory factory;
    ResourceAnalyzer * resanal = factory.newAnalyzer();
    Nepomuk::Resource res;
    resanal->analyze(res);
    QObject::connect(resanal, SIGNAL(analyzingFinished()), &app, SLOT(quit()));
    return app.exec();
#endif
}

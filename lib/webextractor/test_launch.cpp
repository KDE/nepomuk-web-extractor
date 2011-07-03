//#include "webexservice.cpp"
#include <QtCore/QCoreApplication>
#include <QtCore/QList>
#include <QtCore/QVariant>
#include <Nepomuk/Resource>
#include "resourceanalyzerfactory.h"
using namespace std;
using namespace Nepomuk::WebExtractor;
int main(int argc, char ** argv)
{
    QCoreApplication app(argc,argv);
    QList<QVariant> noth;
    ResourceAnalyzerFactory factory;
    ResourceAnalyzer * resanal = factory.newAnalyzer();
    Nepomuk::Resource res;
    resanal->analyze(res);
    return app.exec();

}

#include <QApplication>
#include "graphvizmainwindow.h"
#include <klocalizedstring.h>
#include <KApplication>
#include <KCmdLineArgs>
#include <QByteArray>
#include <KAboutData>
#include <KCmdLineOptions>

int main(int argc, char ** argv)
{
    KAboutData about("nepomuk_graphviz", 0,
                     ki18nc("@title", "NepomukGraphViz "),
                     "1.5",
                     ki18nc("@title", "Nepomuk GraphViz Visualizer"),
                     KAboutData::License_GPL,
                     ki18nc("@info:credit", "(C) 2010 Artem Serebriyskiy"));


    KCmdLineArgs::init(argc, argv, &about);

    KApplication app;

    GraphVizMainWindow * w = new GraphVizMainWindow();
    w->show();
    app.exec();

}


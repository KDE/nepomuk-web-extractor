#include <QApplication>
#include "console.h"
#include <klocalizedstring.h>
#include <KApplication>
#include <KCmdLineArgs>
#include <QByteArray>
#include <KAboutData>
#include <KCmdLineOptions>

int main(int argc, char ** argv)
{
    KAboutData about("webextracor_console", 0,
                     ki18nc("@title", "WebExtractorConsole"),
                     "1.5",
                     ki18nc("@title", "WebExtracor Console"),
                     KAboutData::License_GPL,
                     ki18nc("@info:credit", "(C) 2010 Artem Serebriyskiy"));

    KCmdLineArgs::init(argc, argv, &about);


    KCmdLineOptions options;
    options.add(QByteArray("uri < nepomuk uri >"), ki18n("Nepomuk uri for extracting"));
    options.add("start", ki18n("Start extracting immidiately"));
    options.add("!+datapp", ki18n("Desired set of datapps. Unexisting and invalid will be silently ignored"));

    KCmdLineArgs::addCmdLineOptions(options);

    KApplication app;

    KCmdLineArgs * args = KCmdLineArgs::parsedArgs();

    bool autostart = args->isSet("start");
    QString uri;
    if(args->isSet("uri"))
        uri = args->getOption("uri");

    QStringList datapps;
    for(int i = 0; i < args->count(); i++)
        datapps << args->arg(i);

    kDebug() << "Selected DataPP: " << datapps;


    ConsoleMainWindow * w = new ConsoleMainWindow(uri, datapps, autostart);
    w->show();
    app.exec();

}

#include <QApplication>
#include "console.h"

int main(int argc, char ** argv)
{
    QApplication app(argc,argv);
    ConsoleMainWindow * w = new ConsoleMainWindow();
    w->show();
    app.exec();

}

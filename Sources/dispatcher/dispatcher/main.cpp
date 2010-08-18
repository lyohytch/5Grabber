#include <QApplication>

#include "cmaindispatcher.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    CMainDispatcher dispatcher;
    dispatcher.init();
    return app.exec();
}

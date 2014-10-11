#include "mainwindow.h"
#include <QApplication>

#include "systemtray.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SystemTray tray;
    tray.show();
    return a.exec();
}

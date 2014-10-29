#include <QApplication>

#include "systemtray.h"
#include "agent.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Agent agent;
    SystemTray tray;
    tray.show();
    return a.exec();
}

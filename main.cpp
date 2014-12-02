#include <QApplication>
#include <QDebug>

#include "systemtray.h"
#include "agent.h"
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    Agent agent;
    Q_UNUSED(agent)

    SystemTray tray;
    tray.show();

    return a.exec();
}

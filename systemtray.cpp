#include <QDebug>
#include <QMenu>
#include <QString>
#include <QStringList>
#include <QActionGroup>

#include "systemtray.h"

SystemTray::SystemTray(QObject *parent) :
    QSystemTrayIcon(parent)
{
//    QIcon::setThemeName("Oxygen");
    qDebug() << "Setting icon";
    setIcon(QIcon(":/icons/network-wired.png"));
    buildMenu();
}

void SystemTray::buildMenu()
{
    setContextMenu(new QMenu());
    addTechnologies();
    contextMenu()->addSeparator();
    addServices();
}


void SystemTray::addTechnologies()
{
    QStringList  techs;
    techs << "Wired" << "Wifi" << "Bluetooth";

    foreach (QString tech, techs)
    {
        QAction *action = contextMenu()->addAction(tech);
        action->setCheckable(true);
    }
}

void SystemTray::addServices()
{
    QStringList services;
    services << "Essid1" << "Essid2" << "Essid3";

    QActionGroup *actionGroup = new QActionGroup(this);

    foreach(QString service, services)
    {
        QAction *action = contextMenu()->addAction(service);
        action->setActionGroup(actionGroup);
        action->setCheckable(true);
    }
}

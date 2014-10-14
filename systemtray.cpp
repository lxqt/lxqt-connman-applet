#include <QDebug>
#include <QMenu>
#include <QString>
#include <QStringList>
#include <QActionGroup>

#include "systemtray.h"


SystemTray::SystemTray(QObject *parent) :
    QSystemTrayIcon(parent)
{
    registerDBusTypes();
//    QIcon::setThemeName("Oxygen");
    qDebug() << "Setting icon";
    setIcon(QIcon(":/icons/network-wired.png"));
    buildMenu();

    qDebug() << "Calling connman";

    QString service("net.connman");
    QString objectPath = "/";

    NetConnmanManagerInterface ni(service, objectPath, QDBusConnection::systemBus());
/*    QDBusPendingReply<QMap<QDBusObjectPath, QVariantMap> > pr = ni.GetTechnologies();
    //qDebug() << "pendingreply: "  << pr;

    QDBusMessage m = pr.reply();
    qDebug() << "Signature: " << m.signature();

    QMap<QDBusObjectPath, QVariantMap> map = pr.value();
    foreach (QDBusObjectPath path, map.keys()) {
        qDebug() << path.path();
    }*/
    QVariantMap props = ni.GetProperties();

    qDebug() << "props: " << props;

    ObjectPropertiesList opm = ni.GetTechnologies();
    QPair<QDBusObjectPath, QVariantMap> pair;
    foreach (pair, opm) {
        qDebug() << pair.first.path() << ":" << pair.second;
    }
}

void SystemTray::registerDBusTypes() {
    qDBusRegisterMetaType<ObjectProperties>();
    qDBusRegisterMetaType<ObjectPropertiesList>();
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

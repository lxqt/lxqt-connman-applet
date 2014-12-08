#include <QDebug>
#include <QMenu>
#include <QString>
#include <QStringList>
#include <QActionGroup>
#include <QVariant>

#include "manager.h"
#include "technology.h"
#include "connectionstate.h"
#include "iconfinder.h"

#include "systemtray.h"


SystemTray::SystemTray(QObject *parent) : QSystemTrayIcon(parent),
    technologyEntries(this), serviceEntries(this), quitAction(tr("Quit"), this), trayIcon(":/icons/network-wired.png")
{
    Manager::instance();
    QIcon::setThemeName("Oxygen");
    qDebug() << "Setting icon";
    setConnectionIcon();
    technologyEntries.setExclusive(false);

    setContextMenu(new QMenu());
    connect(contextMenu(), SIGNAL(aboutToShow()), this, SLOT(buildMenu()));
    connect(&quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(&technologyEntries, SIGNAL(triggered(QAction*)), this, SLOT(onTechnologyClicked(QAction*)));
    connect(&serviceEntries, SIGNAL(triggered(QAction*)), this, SLOT(onServiceClicked(QAction*)));
}


void SystemTray::buildMenu()
{
    qDebug() << "Building menu..." ;
    contextMenu()->clear();

    contextMenu()->addSection("Technologies: ");
    foreach (Technology* technology, Manager::instance().technologies())
    {
        QAction *action = contextMenu()->addAction(technology->name());
        action->setCheckable(true);
        action->setChecked(technology->powered());
        technologyEntries.addAction(action);
        action->setData(QVariant::fromValue<QDBusObjectPath>(technology->path()));
    }

    contextMenu()->addSection("Services: ");
    foreach (Service* service, Manager::instance().services())
    {
        QAction *action = contextMenu()->addAction(service->name());
        action->setCheckable(true);
        action->setChecked(QString("online") == service->state() || QString("ready") == service->state());
        serviceEntries.addAction(action);
        action->setData(QVariant::fromValue<QDBusObjectPath>(service->path()));
    }

    contextMenu()->addAction(&quitAction);

}


void SystemTray::onTechnologyClicked(QAction *action)
{
    QDBusObjectPath path = action->data().value<QDBusObjectPath>();
    qDebug() << path.path() << "clicked";

    Technology *technology = Manager::instance().technology(path) ;

    if (! technology)
    {
        qWarning() << "Invalid technology clicked:" << path.path();
        return;
    }

    technology->togglePowered();
}

void SystemTray::onServiceClicked(QAction *action)
{
    QDBusObjectPath path = action->data().value<QDBusObjectPath>();
    Service* service = Manager::instance().service(path);

    if (! service)
    {
        qWarning() << "Invalid service clicked:" << path.path();
        return;
    }

    qDebug() << "Service" << service->name() << "clicked";
    qDebug() << "State:" << service->state();

    if (service->state() == "idle" || service->state() == "failure")
    {
        qDebug() << "Connect...";
        service->Connect();
    }
    else if (service->state() == "online")
    {
        qDebug() << "disconnect...";
        service->disconnect();
    }
}

void SystemTray::setConnectionIcon()
{
    // FIXME setIcon(IconFinder::instance()->icon());
	setIcon(QIcon::fromTheme("preferences-system-network"));
}

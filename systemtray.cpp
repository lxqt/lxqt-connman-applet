#include <QDebug>
#include <QMenu>
#include <QString>
#include <QStringList>
#include <QActionGroup>

#include "manager.h"
#include "technology.h"

#include "systemtray.h"


SystemTray::SystemTray(QObject *parent) : QSystemTrayIcon(parent), technologyEntries(this), serviceEntries(this)
{
    Manager::instance();
    QIcon::setThemeName("Oxygen");
    qDebug() << "Setting icon";
    setIcon(QIcon(":/icons/network-wired.png"));

    technologyEntries.setExclusive(false);

    setContextMenu(new QMenu());
    connect(contextMenu(), SIGNAL(aboutToShow()), this, SLOT(buildMenu()));
}


void SystemTray::buildMenu()
{
    contextMenu()->clear();
    contextMenu()->addSection("Technologies:");
    foreach (TechnologyPtr tech, Technology::technologies())
    {
        QAction *action = contextMenu()->addAction(tech->name());
        action->setCheckable(true);
        action->setChecked(tech->powered());
        technologyEntries.addAction(action);
        action->setData(QString("technology:") + tech->path().path());
    }
    contextMenu()->addSection("Services:");
    foreach (ServicePtr service, Service::services())
    {
        QAction *action = contextMenu()->addAction(service->name());
        action->setCheckable(true);
        action->setChecked(QString("online") == service->state());
        serviceEntries.addAction(action);
        action->setData(QString("service:") + service->path().path());
    }

}


void SystemTray::onTechnologyClicked(QAction *action)
{
    // TODO
}

void SystemTray::onServiceClicked(QAction *action)
{
    // TODO
}

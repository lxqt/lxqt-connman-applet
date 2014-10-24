#include <QDebug>
#include <QMenu>
#include <QString>
#include <QStringList>
#include <QActionGroup>
#include <QVariant>

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
    connect(&technologyEntries, SIGNAL(triggered(QAction*)), this, SLOT(onTechnologyClicked(QAction*)));
    connect(&serviceEntries, SIGNAL(triggered(QAction*)), this, SLOT(onServiceClicked(QAction*)));
}


void SystemTray::buildMenu()
{
    contextMenu()->clear();
    contextMenu()->addSection("Technologies:");
    foreach (TechnologyPtr technologyPtr, Technology::technologies())
    {
        QAction *action = contextMenu()->addAction(technologyPtr->name());
        action->setCheckable(true);
        action->setChecked(technologyPtr->powered());
        technologyEntries.addAction(action);
        action->setData(QVariant(technologyPtr));
    }
    contextMenu()->addSection("Services:");
    foreach (ServicePtr servicePtr, Service::services())
    {
        QAction *action = contextMenu()->addAction(servicePtr->name());
        action->setCheckable(true);
        action->setChecked(QString("online") == servicePtr->state());
        serviceEntries.addAction(action);
        action->setData(QVariant::fromValue<ServicePtr>(servicePtr));
    }

}


void SystemTray::onTechnologyClicked(QAction *action)
{
    TechnologyPtr technologyPtr = action->data().value<TechnologyPtr>();
    qDebug() << technologyPtr->path().path() << "clicked";
}

void SystemTray::onServiceClicked(QAction *action)
{
    ServicePtr servicePtr = action->data().value<ServicePtr>();
    qDebug() << "Into onServiceClicked, state:" << servicePtr->state();
    if (servicePtr->state() == "idle" || servicePtr->state() == "failure") {
        qDebug() << "Connect...";
        servicePtr->Connect();
    }
    else if (servicePtr->state() == "online") {
        qDebug() << "disconnect...";
        servicePtr->disconnect();
    }
}

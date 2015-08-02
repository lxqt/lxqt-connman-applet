/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * lxqt-connman-applet - a gui frontend for connman
 *
 * Copyright: 2014-2015 Christian Surlykke
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include <QDebug>
#include <QMenu>
#include <QString>
#include <QStringList>
#include <QActionGroup>
#include <QVariant>
#include <LXQt/Settings>
#include <QDBusArgument>

#include "manager.h"
#include "technology.h"
#include "iconproducer.h"
#include "ui_strings.h"

#include "systemtray.h"


SystemTray::SystemTray(QObject *parent) : QSystemTrayIcon(parent),
    technologyEntries(this), serviceEntries(this), quitAction(tr("Quit"), this), trayIcon(":/icons/network-wired.png")
{
    technologyEntries.setExclusive(false);

    setContextMenu(new QMenu());
    connect(contextMenu(), SIGNAL(aboutToShow()), this, SLOT(buildMenu()));
    connect(&quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(&technologyEntries, SIGNAL(triggered(QAction*)), this, SLOT(onTechnologyClicked(QAction*)));
    connect(&serviceEntries, SIGNAL(triggered(QAction*)), this, SLOT(onServiceClicked(QAction*)));

    connect(Manager::instance(), SIGNAL(connectionStateChanged()), this, SLOT(updateIcon()));
    connect(IconProducer::instance(), SIGNAL(iconsChanged()), this, SLOT(updateIcon()));
    updateIcon();
}

void SystemTray::updateIcon()
{
    switch (Manager::instance()->connectionState())
    {
    case Manager::Disconnected:
        setIcon(IconProducer::instance()->disconnected());
        break;
    case Manager::Connected_Wired:
        setIcon(IconProducer::instance()->wired_connected());
        break;
    default:
        setIcon(IconProducer::instance()->wireless(Manager::instance()->signalStrength()));
    }
}

void SystemTray::buildMenu()
{
    qDebug() << "Building menu..." ;
    contextMenu()->clear();

    contextMenu()->addSection(tr("Technologies:"));
    foreach (Technology* technology, Manager::instance()->technologies())
    {
        QAction *action = contextMenu()->addAction(uiString(technology->name()));
        action->setCheckable(true);
        action->setChecked(technology->powered());
        technologyEntries.addAction(action);
        action->setData(QVariant::fromValue<QDBusObjectPath>(technology->path()));
    }

    contextMenu()->addSection(tr("Services: "));
    foreach (Service* service, Manager::instance()->services())
    {
        QAction *action;
        if (service->type() == "wifi")
        {
            QIcon icon = IconProducer::instance()->wireless(service->signalStrength());
            action = contextMenu()->addAction(icon, service->name()); // Service name is an ESSID - so no translation
        }
        else if (service->type() == "ethernet")
        {
            action = contextMenu()->addAction(QString("%1 (%2)")
                                                .arg(uiString(service->name()))
                                                .arg(uiString(service->interfaceName())));
        }
        else
        {
            action = contextMenu()->addAction(uiString(service->name()));
        }
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

    Technology *technology = Manager::instance()->technology(path) ;

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
    Service* service = Manager::instance()->service(path);

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


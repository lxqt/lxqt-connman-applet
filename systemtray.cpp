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
#include <QLabel>

#include <QVariant>
#include <LXQt/Settings>
#include <QDBusArgument>

#include "manager.h"
#include "technology.h"
#include "iconproducer.h"
#include "ui_strings.h"

#include "systemtray.h"


SystemTray::SystemTray(QObject *parent) :
    QSystemTrayIcon(parent),
    technologyEntries(this),
    serviceEntries(this),
    technologyHeading(this),
    servicesHeading(this),
    quitAction(tr("Quit"), this),
    trayIcon(":/icons/network-wired.png")
{
    technologyEntries.setExclusive(false);
    QLabel* technologyHeadingLabel = new QLabel("<b>" + tr("Technologies:") + "</b>");
    technologyHeadingLabel->setMargin(5);
    technologyHeading.setDefaultWidget(technologyHeadingLabel);

    QLabel* servicesHeadingLabel = new QLabel("<b>" + tr("Services:") + "</b>");
    servicesHeadingLabel->setMargin(5);
    servicesHeading.setDefaultWidget(servicesHeadingLabel);

    quitAction.setIcon(QIcon::fromTheme("application-exit"));

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
    contextMenu()->addAction(&technologyHeading);
    foreach (Technology* technology, Manager::instance()->technologies())
    {
        QAction *action = contextMenu()->addAction(uiString(technology->name()));
        action->setCheckable(true);
        action->setChecked(technology->powered());
        technologyEntries.addAction(action);
        action->setData(QVariant::fromValue<QDBusObjectPath>(technology->path()));
    }
    contextMenu()->addSeparator();
    contextMenu()->addAction(&servicesHeading);
    foreach (Service* service, Manager::instance()->services())
    {
        QAction* action = contextMenu()->addAction("");
        serviceEntries.addAction(action);
        update(action, service);

    }
    contextMenu()->addSeparator();
    contextMenu()->addAction(&quitAction);

}

void SystemTray::update(QAction *action, Service *service)
{
    QString actionText;
    QIcon actionIcon;

    if (service->type() == "wifi")
    {
        actionIcon = IconProducer::instance()->wireless(service->signalStrength());
        actionText = service->name();
    }
    else if (service->type() == "ethernet")
    {
        actionText = QString("%1 (%2)").arg(uiString(service->name())).arg(uiString(service->interfaceName()));
    }
    else
    {
        actionText = uiString(service->name());
    }

    if (service->state() == "association")
    {
        actionText.append(" (a)");
    }
    else if (service->state() == "configuration")
    {
        actionText.append(" (c)");
    }
    else if (service->state() == "ready")
    {
        actionText.append(" (R)");
    }

    action->setIcon(actionIcon);
    action->setText(actionText);
    action->setData(QVariant::fromValue<QDBusObjectPath>(service->path()));
    action->setCheckable(true);
    action->setChecked(service->state() == "online");
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
    else
    {
        qDebug() << "disconnect...";
        service->disconnect();
    }
}


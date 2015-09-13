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

#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QSystemTrayIcon>
#include <QActionGroup>
#include <QWidgetAction>
#include <QList>

#include "dbus_types.h"

#include "net.connman.Manager.h"
#include "net.connman.Service.h"
#include "net.connman.Technology.h"

class Service;

class SystemTray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit SystemTray(QObject *parent = 0);

public slots:
    void updateIcon();

private slots:
    void buildMenu();
    void onTechnologyClicked(QAction* action);
    void onServiceClicked(QAction* action);

private:
    QActionGroup technologyEntries;
    QActionGroup serviceEntries;
    QWidgetAction technologyHeading;
    QWidgetAction servicesHeading;
    QAction quitAction;
    QIcon trayIcon;

};

#endif // SYSTEMTRAY_H

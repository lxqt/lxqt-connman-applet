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

#include "iconproducer.h"
#include "strings.h"

#include "systemtray.h"


SystemTray::SystemTray(QObject *parent) :
    QSystemTrayIcon(parent)
{
    setIcon(IconProducer::instance()->disconnected());

    QMenu *menu = new QMenu();
    QAction *showServicesAction = menu->addAction(tr("Services..."));
    QAction *aboutAction = menu->addAction(tr("About"));
    QAction *quitAction = menu->addAction(tr("Quit"));
    setContextMenu(menu);

    connect(showServicesAction, SIGNAL(triggered(bool)), SIGNAL(showServicesWindow()));
    connect(aboutAction, SIGNAL(triggered(bool)), SLOT(about()));
    connect(quitAction, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
}

void SystemTray::about()
{
    qDebug() << "about";
}



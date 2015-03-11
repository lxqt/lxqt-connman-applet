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

#ifndef MANAGER_H
#define MANAGER_H

#include <QSharedPointer>
#include <QDBusMessage>

#include "dbus_types.h"
#include "net.connman.Manager.h"
#include "technology.h"
#include "service.h"

// Singleton
class Manager : public NetConnmanManagerInterface
{
    Q_OBJECT

public:

    enum State
    {
        Disconnected,
        Connected_Wired,
        Connected_Wireless
    };

    static Manager* instance();

    TechnologyList technologies() { return mTechnologyMap.values(); }
    Technology* technology(QDBusObjectPath technologyPath) { return mTechnologyMap.value(technologyPath); }
    ServiceList services() { return mServiceList; }
    Service* service(QDBusObjectPath servicePath) { return mServiceMap.value(servicePath); }

    State connectionState() {return mConnectionState;}
    int signalStrength() {return mSignalStrength; }

signals:
    void connectionStateChanged();

private slots:
    void onConnmanStarted(QString serviceName);
    void onConnmanStopped(QString serviceName);
    void onTechnologyAdded(QDBusObjectPath path, QVariantMap properties);
    void onTechnologyRemoved(QDBusObjectPath path);
    void onServicesChanged(ObjectPropertiesList changed, QList<QDBusObjectPath> removed);
    void onServiceStateChange();

private:
    Manager();

    State mConnectionState;
    int mSignalStrength;

    QMap<QDBusObjectPath, Technology*> mTechnologyMap;
    QMap<QDBusObjectPath, Service*> mServiceMap;
    QList<Service*> mServiceList;
    QSet<Service*> mConnectedServices;

};

#endif // MANAGER_H

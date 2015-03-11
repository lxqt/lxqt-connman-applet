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

#include <QList>

#include "manager.h"
#include "technology.h"

Manager *Manager::instance()
{
    static Manager *instance = 0;
    if (! instance)
    {
        qDBusRegisterMetaType<ObjectProperties>();
        qDBusRegisterMetaType<ObjectPropertiesList>();
        qDBusRegisterMetaType<QList<QDBusObjectPath> >();
        instance = new Manager();
    }


    return instance;
}


Manager::Manager() :
    NetConnmanManagerInterface("net.connman", "/", QDBusConnection::systemBus())
{
    mSignalStrength = 0;

    connect(this, SIGNAL(ServicesChanged(ObjectPropertiesList,QList<QDBusObjectPath>)), this, SLOT(onServicesChanged(ObjectPropertiesList,QList<QDBusObjectPath>)));

    foreach (ObjectProperties op, ObjectPropertiesList(GetTechnologies()))
    {
        onTechnologyAdded(op.first, op.second);
    }

    QDBusServiceWatcher *watcher = new QDBusServiceWatcher("net.connman", QDBusConnection::systemBus(),
                                                           QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration);
    connect(watcher, SIGNAL(serviceRegistered(QString)), this, SLOT(onConnmanStarted(QString)));
    connect(watcher, SIGNAL(serviceUnregistered(QString)), this, SLOT(onConnmanStopped(QString)));

    onServicesChanged(ObjectPropertiesList(GetServices()), QList<QDBusObjectPath>());

}


void Manager::onConnmanStarted(QString serviceName)
{
    qDebug() << "Connman started:" << serviceName;
}

void Manager::onConnmanStopped(QString serviceName)
{
    qDebug() << "Connman stopped" << serviceName;
}

void Manager::onTechnologyAdded(QDBusObjectPath path, QVariantMap properties)
{
    qDebug() << "Technology added:" << path.path();
    mTechnologyMap[path] = new Technology(path, properties);
}

void Manager::onTechnologyRemoved(QDBusObjectPath path)
{
    if (mTechnologyMap.contains(path))
    {
        qDebug() << "removing:" << path.path();
        mTechnologyMap.take(path)->deleteLater();
    }
}

void Manager::onServicesChanged(ObjectPropertiesList changed, QList<QDBusObjectPath> removed)
{

    qDebug() << "Services changed";

    mServiceList.clear();

    foreach (QDBusObjectPath pathOfRemovedService, removed)
    {
        if (mServiceMap.contains(pathOfRemovedService)) {
            qDebug() << "Removing" << *service(pathOfRemovedService);
            mServiceMap.take(pathOfRemovedService)->deleteLater();
        }
    }

    qDebug() << "Merging" << changed.size();
    foreach (ObjectProperties objectProperties, changed)
    {
        QDBusObjectPath servicePath = objectProperties.first;
        QVariantMap serviceProperties = objectProperties.second;

        qDebug() << servicePath.path();

        if (!mServiceMap.contains(servicePath))
        {
            Service *service = new Service(servicePath);
            connect(service, SIGNAL(stateChanged(Service*)), this, SLOT(onServiceStateChange()));
            qDebug() << "Adding" << *service;
            mServiceMap.insert(servicePath, service);
        }

        Service* service = mServiceMap[servicePath];

        qDebug() << "Setting properties on " << service;
        foreach (QString propertyKey, serviceProperties.keys())
        {
            qDebug() << "Setting" << propertyKey << ":" << serviceProperties[propertyKey];
            service->setProperty(propertyKey, serviceProperties[propertyKey]);
        }

        qDebug() << "Appending" << servicePath.path();
        mServiceList.append(service);
    }
    qDebug() << "Merging done";

    onServiceStateChange();
}


void Manager::onServiceStateChange()
{
    QSet<Service*> connectedServicesNow;
    bool somethingChanged = false;

    foreach (Service *s, services())
    {
        if (s->state() == "online" || s->state() == "ready")
        {
            connectedServicesNow << s;
        }
    }

    mConnectionState = connectedServicesNow.isEmpty() ? Disconnected : Connected_Wired;

    if (connectedServicesNow.size() != mConnectedServices.size())
    {
        somethingChanged = true;
    }

    foreach (Service *s, connectedServicesNow)
    {
        if (! mConnectedServices.contains(s))
        {
            somethingChanged = true;
        }

        if (s->type() == "wifi")
        {
            mConnectionState = Connected_Wireless;
            if (s->signalStrength() != mSignalStrength)
            {
                somethingChanged = true;
                mSignalStrength = s->signalStrength();
            }
        }
    }


    mConnectedServices.clear();
    mConnectedServices = connectedServicesNow;

    if (somethingChanged)
    {
        emit connectionStateChanged();
    }

}

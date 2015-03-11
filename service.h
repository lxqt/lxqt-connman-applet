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

#ifndef SERVICE_H
#define SERVICE_H

#include <QSharedPointer>
#include <QVariantMap>

#include "dbus_types.h"
#include "net.connman.Service.h"



class Service : public NetConnmanServiceInterface
{
    Q_OBJECT

public:
    Service(QDBusObjectPath mPath);

    QDBusObjectPath path() { return mPath;}
    QString state() { return mProperties.value("State").toString(); }
    QString name() { return mProperties.value("Name").toString(); }
    QString type() { return mProperties.value("Type").toString(); }
    int signalStrength() { return mProperties.value("Strength").toInt(); }
    QString interfaceName();

    void setProperty(QString key, QVariant newValue);
    QVariant getProperty(QString key);

signals:
    void stateChanged(Service* service);

private slots:
    void onPropertyChange(QString key, QDBusVariant newValue);

private:
    QVariantMap mProperties;
    const QDBusObjectPath mPath;
};

QDebug operator<<(QDebug dbg, Service &service);

typedef QList<Service*> ServiceList;


#endif // SERVICE_H

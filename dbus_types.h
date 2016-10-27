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

#ifndef DBUS_TYPES_H
#define DBUS_TYPES_H

#include <QMetaType>
#include <QMap>
#include <QDBusInterface>
#include <QDBusObjectPath>
#include <QVariantMap>
#include <QDBusMetaType>
#include <QDBusConnection>
#include <QDebug>

typedef QPair<QDBusObjectPath, QVariantMap> ObjectProperties;
typedef QList<ObjectProperties> ObjectPropertiesList;

Q_DECLARE_METATYPE(ObjectProperties)
Q_DECLARE_METATYPE(ObjectPropertiesList)

QDebug operator<<(QDebug debug, const QDBusObjectPath &path);

struct RemoteObject : public QDBusInterface
{
    RemoteObject(const QString& path, const QString& interface, const QVariantMap& initialProperties);
    QVariantMap properties;
};

struct Service : public RemoteObject
{
    Service(const QString& path, const QVariantMap& initialProperties);
};

struct Technology : public RemoteObject
{
    Technology(const QString& path, const QVariantMap& initialProperties);
};

struct Manager : public RemoteObject
{
    Manager();
};





#endif // DBUS_TYPES_H

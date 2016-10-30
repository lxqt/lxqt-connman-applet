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
#include <QVariantMap>
#include <QDBusMetaType>
#include <QDBusConnection>
#include <QDBusObjectPath>
#include <QDBusAbstractInterface>
#include <QDebug>

typedef QPair<QDBusObjectPath, QVariantMap> ObjectProperties;
typedef QList<ObjectProperties> ObjectPropertiesList;

Q_DECLARE_METATYPE(ObjectProperties)
Q_DECLARE_METATYPE(ObjectPropertiesList)

extern bool dbus_types_registered;


class ConnmanObject : public QDBusAbstractInterface, public QVariantMap
{
    Q_OBJECT

public:
    ConnmanObject(const QString& path, const char* interface, const QVariantMap properties = QVariantMap());

Q_SIGNALS:
   void PropertyChanged(const QString& name, const QDBusVariant& newValue);

protected slots:
   virtual void onPropertyChanged(const QString& name, const QDBusVariant& newValue) {}
};

Q_DECLARE_METATYPE(ConnmanObject*)

class ConnmanManager : public ConnmanObject
{
    Q_OBJECT

public:
    ConnmanManager(QObject* parent = 0) : ConnmanObject("/", "net.connman.Manager") {}

Q_SIGNALS:
    void TechnologyAdded(const QDBusObjectPath& object, const QVariantMap& properties);
    void TechnologyRemoved(const QDBusObjectPath& object);
    void ServicesChanged(ObjectPropertiesList added, const QList<QDBusObjectPath>& removed);
};


#endif // DBUS_TYPES_H

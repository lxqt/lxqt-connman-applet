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

#include "service.h"

Service::Service(QDBusObjectPath path) :
    NetConnmanServiceInterface("net.connman", path.path(), QDBusConnection::systemBus()),
    mPath(path),
    mProperties()
{
    connect(this, SIGNAL(PropertyChanged(QString,QDBusVariant)), this, SLOT(onPropertyChange(QString,QDBusVariant)));
}

QString Service::interfaceName()
{
    if (type() != "ethernet")
    {
        return QString();
    }

    QDBusArgument arg = mProperties["Ethernet"].value<QDBusArgument>();
    QVariantMap map;
    arg >> map;
    return map["Interface"].toString();
}

void Service::setProperty(QString key, QVariant newValue)
{
    mProperties[key] = newValue;
}

void Service::onPropertyChange(QString key, QDBusVariant newValue)
{
    qDebug() << "Service::onPropertyChange" << key << ":" << newValue.variant();
    setProperty(key, newValue.variant());
    if (key == "State")
    {
        emit stateChanged(this);
    }
}


QDebug operator<<(QDebug dbg, Service &service)
{
    dbg.nospace() << "Service(" << service.path().path() << ")";
    return dbg.maybeSpace();
}

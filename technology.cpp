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

#include <QDBusConnection>
#include "technology.h"

void Technology::togglePowered()
{
    bool newState = !powered();
    qDebug() << "Setting power" << newState << "on" << *this;

    QDBusMessage reply = SetProperty("Powered", QDBusVariant(newState)).reply();
    qDebug() << "reply:" << reply.errorMessage();
}


Technology::Technology(QDBusObjectPath path, QVariantMap properties) :
    NetConnmanTechnologyInterface("net.connman", path.path(), QDBusConnection::systemBus()), mPath(path), mProperties(properties)
{
    connect(this, SIGNAL(PropertyChanged(QString,QDBusVariant)), this, SLOT(onPropertyChange(QString,QDBusVariant)));
    Scan();
}

void Technology::onPropertyChange(QString key, QDBusVariant newValue)
{
    qDebug() << "Technology::onPropertyChange" << key << ":" << newValue.variant();
    bool shouldScan = key == "Powered" && !mProperties.value(key, false).toBool() && newValue.variant().toBool();
    mProperties.insert(key, newValue.variant());

    if (shouldScan) {
        Scan();
    }
}


QDebug operator <<(QDebug dbg, Technology &technology)
{
    dbg.nospace() << "Technology(" << technology.path().path() << ")";
    return dbg.maybeSpace();
}

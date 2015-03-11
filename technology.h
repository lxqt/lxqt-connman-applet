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

#ifndef TECHNOLOGY_H
#define TECHNOLOGY_H

#include <QSharedPointer>
#include <QVariantMap>

#include "net.connman.Technology.h"


class Technology : public NetConnmanTechnologyInterface
{
    Q_OBJECT

public:
    Technology(QDBusObjectPath path, QVariantMap properties);

    QDBusObjectPath path() { return mPath; }
    bool powered() { return mProperties.value("Powered").toBool(); }
    bool connected() { return mProperties.value("Connected").toBool(); }
    QString name() { return mProperties.value("Name").toString(); }
    QString type() { return mProperties.value("Type").toString(); }
    void togglePowered();

private slots:
    void onPropertyChange(QString key, QDBusVariant newValue);

private:
    void scanIfNeedBe();
    const QDBusObjectPath mPath;
    QVariantMap mProperties;


};

QDebug operator <<(QDebug dbg, Technology &technology);

typedef QList<Technology*> TechnologyList;

#endif // TECHNOLOGY_H

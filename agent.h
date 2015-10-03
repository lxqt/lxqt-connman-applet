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

#ifndef AGENT_H
#define AGENT_H

#include <QDBusContext>
#include <QDBusObjectPath>
#include <QDebug>
#include "net.connman.Manager.h"

class Agent : public QObject, protected QDBusContext
{
    Q_OBJECT
public:
    explicit Agent();

    void Release();
    void Cancel();
    void ReportError(QDBusObjectPath service, QString errorMessage);
    void ReportPeerError(QDBusObjectPath peer, QString errorMessage);
    void RequestBrowser(QDBusObjectPath service, QString url);
    QVariantMap RequestInput(QDBusObjectPath servicePath, QVariantMap fields);
    QVariantMap RequestPeerAuthorization(QDBusObjectPath peer, QVariantMap fields);
    QString Introspect();

signals:
    void operationCanceled();

private:
    QString getName(QDBusObjectPath servicePath);
    NetConnmanManagerInterface managerInterface;
    QDBusObjectPath path;
};

#endif // AGENT_H

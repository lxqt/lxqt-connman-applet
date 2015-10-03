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

#include <QDBusObjectPath>
#include <QDebug>

#include "agent.h"
#include "agentadaptor.h"
#include "dialog.h"

Agent::Agent() :
    QObject(),
    path("/org/lxqt/lxqt_connman_agent"),
    managerInterface("net.connman", "/", QDBusConnection::systemBus())
{
    QDBusConnection::systemBus().registerService("org.lxqt.lxqt_connman_agent");

    new AgentAdaptor(this);
    new IntrospectableAdaptor(this);

    QDBusConnection::systemBus().registerObject(path.path(), this);

    managerInterface.RegisterAgent(path);

 }

void Agent::Release()
{
    // Nothing to do
}

void Agent::Cancel()
{
    qDebug() << "Canceled";
}

void Agent::ReportError(QDBusObjectPath service, QString errorMessage)
{
    // FIXME
}

void Agent::ReportPeerError(QDBusObjectPath peer, QString errorMessage)
{
    // FIXME
}

void Agent::RequestBrowser(QDBusObjectPath service, QString url)
{
    // FIXME
}

QVariantMap Agent::RequestInput(QDBusObjectPath servicePath, QVariantMap fields)
{
    QString serviceName = getName(servicePath);
    if (serviceName.isEmpty())
    {
        sendErrorReply("net.connman.Agent.Error.Canceled", "Unknown service");
        return QVariantMap();
    }

    Dialog infoDialog(serviceName, fields);
    connect(this, SIGNAL(operationCanceled()), &infoDialog, SLOT(reject()));

    if (Dialog::Rejected == infoDialog.exec())
    {
        sendErrorReply("net.connman.Agent.Error.Canceled", "Cancelled");
        return QVariantMap();
    }

    return infoDialog.collectedInput();
}

QVariantMap Agent::RequestPeerAuthorization(QDBusObjectPath peer, QVariantMap fields)
{
    return QVariantMap(); // FIXME
}


QString Agent::Introspect()
{
    qDebug() << "Introspect...";
    QFile xmlFile(":/interface/net.connman.Agent.xml");
    xmlFile.open(QIODevice::ReadOnly);
    QString xml(xmlFile.readAll());
    return xml;
}

QString Agent::getName(QDBusObjectPath servicePath)
{

}

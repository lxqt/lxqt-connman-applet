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

#include <QDebug>
#include <QDBusReply>
#include "controller.h"
#include "agentdialog.h"
#include "agent.h"

Agent::Agent() : QObject()
{
}

void Agent::Release()
{
    // Nothing to do
}

void Agent::Cancel()
{
    qDebug() << "Canceled";
}

void Agent::ReportError(QDBusObjectPath servicePath, QString errorMessage)
{
    // FIXME
}

void Agent::ReportPeerError(QDBusObjectPath peerPath, QString errorMessage)
{
    // FIXME
}

void Agent::RequestBrowser(QDBusObjectPath servicePath, QString url)
{
    // FIXME
}

QVariantMap Agent::RequestInput(QDBusObjectPath servicePath, QVariantMap fields)
{
    QString serviceName = serviceNames.value(servicePath.path(), servicePath.path());
    if (serviceName.isEmpty())
    {
        sendErrorReply("net.connman.Agent.Error.Canceled", "Unknown service");
        return QVariantMap();
    }

    AgentDialog agentDialog(serviceName, fields);
    connect(this, SIGNAL(operationCanceled()), &agentDialog, SLOT(reject()));

    if (AgentDialog::Rejected == agentDialog.exec())
    {
        sendErrorReply("net.connman.Agent.Error.Canceled", "Cancelled");
        return QVariantMap();
    }
    return agentDialog.collectedInput();
}

QVariantMap Agent::RequestPeerAuthorization(QDBusObjectPath peerPath, QVariantMap fields)
{
    return QVariantMap(); // FIXME
}


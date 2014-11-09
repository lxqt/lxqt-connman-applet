#include <QDBusObjectPath>
#include <QDebug>

#include "agent.h"
#include "agentadaptor.h"
#include "net.connman.Manager.h"
#include "dialog.h"

Agent::Agent() : QObject(), path("/org/lxqt/lxqt_connman_agent")
{
    QDBusConnection bus = QDBusConnection::systemBus();
    bus.registerService("org.lxqt.lxqt_connman_agent");

    new AgentAdaptor(this);
    new IntrospectableAdaptor(this);

    bus.registerObject(path.path(), this);

    NetConnmanManagerInterface netConnmanManagerInterface("net.connman", "/", bus);
    netConnmanManagerInterface.RegisterAgent(path);

 }

void Agent::Release()
{
    // Nothing to do, really
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

QVariantMap Agent::RequestInput(QDBusObjectPath service, QVariantMap fields)
{
    Dialog infoDialog(service.path(), fields);
    connect(this, SIGNAL(operationCanceled()), &infoDialog, SLOT(reject()));
    if (Dialog::Rejected == infoDialog.exec())
    {
        sendErrorReply("net.connman.Agent.Error.Canceled", "Cancelled");
        return QVariantMap();
    }
    else
    {
        return infoDialog.collectedInput();
    }
}

QVariantMap Agent::RequestPeerAuthorization(QDBusObjectPath peer, QVariantMap fields)
{
    return QVariantMap(); // FIXME
}


QString Agent::Introspect()
{
    qDebug() << "Ind i Introspect...";
    QFile xmlFile(":/interface/net.connman.Agent.xml");
    xmlFile.open(QIODevice::ReadOnly);
    QString xml(xmlFile.readAll());
    return xml;
}

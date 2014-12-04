#include <QDBusObjectPath>
#include <QDebug>

#include "agent.h"
#include "agentadaptor.h"
#include "net.connman.Manager.h"
#include "dialog.h"
#include "service.h"
#include "manager.h"

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
    Service *service = Manager::instance().service(servicePath);

    if (!service)
    {
        sendErrorReply("net.connman.Agent.Error.Canceled", "Unknown service");
        return QVariantMap();
    }

    Dialog infoDialog(service->name(), fields);
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

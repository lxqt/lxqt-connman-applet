#include <QDBusObjectPath>
#include "agent.h"
#include "agent_adaptor.h"
#include "net.connman.Manager.h"

Agent::Agent() : QObject(), path("/org/lxqt/lxqt_connman_agent")
{
    qDebug() << "After setPath, path:" << path.path();
    new AgentAdaptor(this);
    qDebug() << "Register object";
    QDBusConnection::systemBus().registerObject(path.path(), this);

    NetConnmanManagerInterface netConnmanManagerInterface("net.connman", "/", QDBusConnection::systemBus());
    qDebug() << "Register agent on" << path.path();
    netConnmanManagerInterface.RegisterAgent(path);

 }

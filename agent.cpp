#include "agent.h"
#include "agent_adaptor.h"
#include "net.connman.Manager.h"

#define DBUS_PATH "/org/lxqt/lxqt-connman-agent"

Agent::Agent(int argc, char **argv) : QApplication(argc, argv)
{
    new AgentAdaptor(this);
    QDBusConnection::systemBus().registerObject(DBUS_PATH, this);


    NetConnmanManagerInterface netConnmanManagerInterface("net.connman", "/", QDBusConnection::systemBus());
    netConnmanManagerInterface.RegisterAgent(QDBusObjectPath(DBUS_PATH));

}

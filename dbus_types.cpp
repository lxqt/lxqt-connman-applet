#include "dbus_types.h"

QDebug operator<<(QDebug debug, const QDBusObjectPath &path)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << path.path();

    return debug;
}

RemoteObject::RemoteObject(const QString& path, const QString& interface, const QVariantMap& initialProperties) :
    QDBusInterface("net.connman", path, interface, QDBusConnection::systemBus()),
    properties(initialProperties)
{
}

Service::Service(const QString& path, const QVariantMap& initialProperties) :
    RemoteObject(path, "net.connman.Service", initialProperties)
{
}

Technology::Technology(const QString& path, const QVariantMap& initialProperties) :
    RemoteObject(path, "net.connman.Technology", initialProperties)
{
}

Manager::Manager() :
    RemoteObject("/", "net.connman.Manager", QVariantMap())
{
    // FIXME Get props
}

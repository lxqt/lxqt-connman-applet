#include "connmannobject.h"

ConnmanObject::ConnmanObject(const QString& path, const char* interface, QObject* parent) :
    QDBusAbstractInterface("net.connman", path, interface, QDBusConnection::systemBus(), parent),
    QVariantMap()
{
}


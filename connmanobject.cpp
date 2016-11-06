#include "connmanobject.h"

bool dbus_types_registered = []() -> bool {
    qDBusRegisterMetaType<ObjectProperties>();
    qDBusRegisterMetaType<ObjectPropertiesList>();
    return true;
}();


ConnmanObject::ConnmanObject(const QString& path, const char* interface, const QVariantMap properties) :
    QDBusAbstractInterface("net.connman", path, interface, QDBusConnection::systemBus(), 0),
    properties(properties)
{
    connect(this, &ConnmanObject::PropertyChanged, this, &ConnmanObject::onPropertyChanged);
}

void ConnmanObject::onPropertyChanged(const QString& name, const QDBusVariant& newValue)
{
    properties.insert(name, newValue.variant());
}

ConnmanManager::ConnmanManager() : ConnmanObject("/", "net.connman.Manager")
{
}

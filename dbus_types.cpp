#include "dbus_types.h"

bool dbus_types_registered = []() -> bool {
    qDBusRegisterMetaType<ObjectProperties>();
    qDBusRegisterMetaType<ObjectPropertiesList>();
    return true;
}();


ConnmanObject::ConnmanObject(const QString& path, const char* interface, const QVariantMap properties) :
    QDBusAbstractInterface("net.connman", path, interface, QDBusConnection::systemBus(), 0),
    QVariantMap(properties)
{
    connect(this, &ConnmanObject::PropertyChanged,
            [this](const QString& name, const QDBusVariant& newValue) {
                insert(name, newValue.variant());
            });
}


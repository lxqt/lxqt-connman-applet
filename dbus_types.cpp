#include "dbus_types.h"

void registerDbusTypes()
{
    qDBusRegisterMetaType<ObjectProperties>();
    qDBusRegisterMetaType<ObjectPropertiesList>();
}

QDebug operator<<(QDebug debug, const QDBusObjectPath &path)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << path.path();

    return debug;
}


void show(const QList<QDBusObjectPath>& paths)
{
    for (const QDBusObjectPath& path : paths) qDebug() << path.path();
}

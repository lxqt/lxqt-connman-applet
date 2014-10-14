#ifndef DBUS_TYPES_H
#define DBUS_TYPES_H

#include <QMetaType>
#include <QMap>
#include <QDBusObjectPath>
#include <QVariantMap>

typedef QMap<QDBusObjectPath, QVariantMap> ObjectPropertyMap;

Q_DECLARE_METATYPE(ObjectPropertyMap)

#endif // DBUS_TYPES_H

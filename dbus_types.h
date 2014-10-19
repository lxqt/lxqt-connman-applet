#ifndef DBUS_TYPES_H
#define DBUS_TYPES_H

#include <QMetaType>
#include <QMap>
#include <QDBusObjectPath>
#include <QVariantMap>
#include <QDBusMetaType>


typedef QPair<QDBusObjectPath, QVariantMap> ObjectProperties;
typedef QList<ObjectProperties> ObjectPropertiesList;
typedef QList<QDBusObjectPath> ObjectPathList;

Q_DECLARE_METATYPE(ObjectProperties)
Q_DECLARE_METATYPE(ObjectPropertiesList)
Q_DECLARE_METATYPE(ObjectPathList)

#endif // DBUS_TYPES_H

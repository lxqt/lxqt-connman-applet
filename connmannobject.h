/*
 * FIXME credit to dbusxml2cpp...
 */
#ifndef CONNMANNOBJECT_H
#define CONNMANNOBJECT_H

#include <QDBusAbstractInterface>
#include <QDBusPendingReply>
#include <QDBusObjectPath>
#include "dbus_types.h"

class ConnmanObject : public QDBusAbstractInterface, public QVariantMap
{
    Q_OBJECT

public:
    ConnmanObject(const QString& path, const char* interface, QObject* parent = 0);

Q_SIGNALS:
   void PropertyChanged(const QString& name, const QDBusVariant& newValue);
};

class ConnmanManager : public ConnmanObject
{
    Q_OBJECT

public:
    ConnmanManager() : ConnmanObject("/", "net.connman.Manager") {}

Q_SIGNALS:
    void TechnologyAdded(const QDBusObjectPath& object, const QVariantMap& properties);
    void TechnologyRemoved(const QDBusObjectPath& object);
    void ServicesChanged(ObjectPropertiesList added, const QList<QDBusObjectPath>& removed);
};


#endif // CONNMANNOBJECT_H

#ifndef MANAGER_H
#define MANAGER_H

#include <QSharedPointer>
#include <QDBusMessage>

#include "dbus_types.h"
#include "net.connman.Manager.h"
#include "technology.h"
#include "service.h"

class Manager : public NetConnmanManagerInterface
{
    Q_OBJECT

public:
    static Manager& instance();

private:
    Manager();

private slots:
    void onTechnologyAdded(QDBusObjectPath path, QVariantMap properties);
    void onTechnologyRemoved(QDBusObjectPath path);
    void onServicesChanged(ObjectPropertiesList changed, QList<QDBusObjectPath> removed);
};

#endif // MANAGER_H

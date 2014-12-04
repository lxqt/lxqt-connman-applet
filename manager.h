#ifndef MANAGER_H
#define MANAGER_H

#include <QSharedPointer>
#include <QDBusMessage>

#include "dbus_types.h"
#include "net.connman.Manager.h"
#include "technology.h"
#include "service.h"

// Singleton
class Manager : public NetConnmanManagerInterface
{
    Q_OBJECT


public:
    static Manager& instance();

    TechnologyList technologies() { return mTechnologyMap.values(); }
    Technology* technology(QDBusObjectPath technologyPath) { return mTechnologyMap.value(technologyPath); }
    ServiceList services() { return mServiceList; }
    Service* service(QDBusObjectPath servicePath) { return mServiceMap.value(servicePath); }

private:
    Manager();
    QMap<QDBusObjectPath, Technology*> mTechnologyMap;
    QMap<QDBusObjectPath, Service*> mServiceMap;
    QList<Service*> mServiceList;

private slots:
    void onConnmanStarted(QString serviceName);
    void onConnmanStopped(QString serviceName);
    void onTechnologyAdded(QDBusObjectPath path, QVariantMap properties);
    void onTechnologyRemoved(QDBusObjectPath path);
    void onServicesChanged(ObjectPropertiesList changed, QList<QDBusObjectPath> removed);
};

#endif // MANAGER_H

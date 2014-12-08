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

    enum State
    {
        Disconnected,
        Connected_Wired,
        Connected_Wireless
    };

    static Manager* instance();

    TechnologyList technologies() { return mTechnologyMap.values(); }
    Technology* technology(QDBusObjectPath technologyPath) { return mTechnologyMap.value(technologyPath); }
    ServiceList services() { return mServiceList; }
    Service* service(QDBusObjectPath servicePath) { return mServiceMap.value(servicePath); }

    State connectionState() {return mConnectionState;}
    int signalStrength() {return mSignalStrength; }

signals:
    void connectionStateChanged();

private slots:
    void onConnmanStarted(QString serviceName);
    void onConnmanStopped(QString serviceName);
    void onTechnologyAdded(QDBusObjectPath path, QVariantMap properties);
    void onTechnologyRemoved(QDBusObjectPath path);
    void onServicesChanged(ObjectPropertiesList changed, QList<QDBusObjectPath> removed);

private:
    Manager();
    bool connectionChanged();
    State mConnectionState;
    int mSignalStrength;

    QMap<QDBusObjectPath, Technology*> mTechnologyMap;
    QMap<QDBusObjectPath, Service*> mServiceMap;
    QList<Service*> mServiceList;

    QSet<Service*> mConnectedServices;

};

#endif // MANAGER_H

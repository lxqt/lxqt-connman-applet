#include <QList>

#include "manager.h"
#include "technology.h"

Manager *Manager::instance()
{
    static Manager *instance = 0;
    if (! instance)
    {
        qDBusRegisterMetaType<ObjectProperties>();
        qDBusRegisterMetaType<ObjectPropertiesList>();
        qDBusRegisterMetaType<QList<QDBusObjectPath> >();
        instance = new Manager();
    }


    return instance;
}


Manager::Manager() :
    NetConnmanManagerInterface("net.connman", "/", QDBusConnection::systemBus())
{
    mSignalStrength = 0;

    connect(this, SIGNAL(ServicesChanged(ObjectPropertiesList,QList<QDBusObjectPath>)), this, SLOT(onServicesChanged(ObjectPropertiesList,QList<QDBusObjectPath>)));

    foreach (ObjectProperties op, ObjectPropertiesList(GetTechnologies()))
    {
        onTechnologyAdded(op.first, op.second);
    }

    QDBusServiceWatcher *watcher = new QDBusServiceWatcher("net.connman", QDBusConnection::systemBus(),
                                                           QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration);
    connect(watcher, SIGNAL(serviceRegistered(QString)), this, SLOT(onConnmanStarted(QString)));
    connect(watcher, SIGNAL(serviceUnregistered(QString)), this, SLOT(onConnmanStopped(QString)));

    onServicesChanged(ObjectPropertiesList(GetServices()), QList<QDBusObjectPath>());

}


void Manager::onConnmanStarted(QString serviceName)
{
    qDebug() << "Connman started:" << serviceName;
}

void Manager::onConnmanStopped(QString serviceName)
{
    qDebug() << "Connman stopped" << serviceName;
}

void Manager::onTechnologyAdded(QDBusObjectPath path, QVariantMap properties)
{
    qDebug() << "Technology added:" << path.path();
    mTechnologyMap[path] = new Technology(path, properties);
}

void Manager::onTechnologyRemoved(QDBusObjectPath path)
{
    if (mTechnologyMap.contains(path))
    {
        qDebug() << "removing:" << path.path();
        mTechnologyMap.take(path)->deleteLater();
    }
}

void Manager::onServicesChanged(ObjectPropertiesList changed, QList<QDBusObjectPath> removed)
{

    qDebug() << "Services changed";

    mServiceList.clear();

    foreach (QDBusObjectPath pathOfRemovedService, removed)
    {
        if (mServiceMap.contains(pathOfRemovedService)) {
            qDebug() << "Removing" << *service(pathOfRemovedService);
            mServiceMap.take(pathOfRemovedService)->deleteLater();
        }
    }

    qDebug() << "Merging" << changed.size();
    foreach (ObjectProperties objectProperties, changed)
    {
        QDBusObjectPath servicePath = objectProperties.first;
        QVariantMap serviceProperties = objectProperties.second;

        qDebug() << servicePath.path();

        if (!mServiceMap.contains(servicePath))
        {
            Service *service = new Service(servicePath);
            qDebug() << "Adding" << *service;
            mServiceMap.insert(servicePath, service);
        }

        Service* service = mServiceMap[servicePath];

        qDebug() << "Setting properties on " << service;
        foreach (QString propertyKey, serviceProperties.keys())
        {
            qDebug() << "Setting" << propertyKey << ":" << serviceProperties[propertyKey];
            service->setProperty(propertyKey, serviceProperties[propertyKey]);
        }

        qDebug() << "Appending" << servicePath.path();
        mServiceList.append(service);
    }
    qDebug() << "Merging done";

    QSet<Service*> connectedServicesNow;
    bool somethingChanged = false;

    foreach (Service *s, services())
    {
        if (s->state() == "online")
        {
            connectedServicesNow << s;
        }
    }

    mConnectionState = connectedServicesNow.isEmpty() ? Disconnected : Connected_Wired;

    if (connectedServicesNow.size() != mConnectedServices.size())
    {
        somethingChanged = true;
    }

    foreach (Service *s, connectedServicesNow)
    {
        if (! mConnectedServices.contains(s))
        {
            somethingChanged = true;
        }

        if (s->type() == "wifi")
        {
            mConnectionState = Connected_Wireless;
            if (s->signalStrength() != mSignalStrength)
            {
                somethingChanged = true;
                mSignalStrength = s->signalStrength();
            }
        }
    }


    mConnectedServices.clear();
    mConnectedServices.unite(connectedServicesNow);

    if (somethingChanged)
    {
        emit connectionStateChanged();
    }
}

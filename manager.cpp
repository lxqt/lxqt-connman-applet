#include <QList>

#include "manager.h"
#include "technology.h"

Manager& Manager::instance()
{
    static Manager *instance = 0;
    if (! instance)
    {
        qDBusRegisterMetaType<ObjectProperties>();
        qDBusRegisterMetaType<ObjectPropertiesList>();
        qDBusRegisterMetaType<QList<QDBusObjectPath> >();
        instance = new Manager();
    }

    return *instance;
}


Manager::Manager() :
    NetConnmanManagerInterface("net.connman", "/", QDBusConnection::systemBus())
{
    qDebug() << "1";
    connect(this, SIGNAL(ServicesChanged(ObjectPropertiesList,QList<QDBusObjectPath>)), this, SLOT(onServicesChanged(ObjectPropertiesList,QList<QDBusObjectPath>)));

    qDebug() << "2";
    foreach (ObjectProperties op, ObjectPropertiesList(GetTechnologies()))
    {
        onTechnologyAdded(op.first, op.second);
    }

    qDebug() << "3";
    QDBusServiceWatcher *watcher = new QDBusServiceWatcher("net.connman", QDBusConnection::systemBus(),
                                                           QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration);
    qDebug() << "4";
    connect(watcher, SIGNAL(serviceRegistered(QString)), this, SLOT(onConnmanStarted(QString)));
    connect(watcher, SIGNAL(serviceUnregistered(QString)), this, SLOT(onConnmanStopped(QString)));


    qDebug() << "5";
    onServicesChanged(ObjectPropertiesList(GetServices()), QList<QDBusObjectPath>());
    qDebug() << "6";

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
            //mServiceMap.take(pathOfRemovedService)->deleteLater();
            mServiceMap[pathOfRemovedService]->deleted = true;
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
        else
        {
            qDebug() << "Was there..";
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
}

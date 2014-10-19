#include <QList>

#include "manager.h"
#include "technology.h"

Manager& Manager::instance()
{
    static bool initialized = false;
    if (! initialized)
    {
        qDBusRegisterMetaType<ObjectProperties>();
        qDBusRegisterMetaType<ObjectPropertiesList>();
        qDBusRegisterMetaType<QList<QDBusObjectPath> >();
        initialized = true;
    }

    static Manager _instance;
    return _instance;
}

Manager::Manager() :
    NetConnmanManagerInterface("net.connman", "/", QDBusConnection::systemBus())
{
    qDebug() << connect(this, SIGNAL(ServicesChanged(ObjectPropertiesList,QList<QDBusObjectPath>)), this, SLOT(onServicesChanged(ObjectPropertiesList,QList<QDBusObjectPath>)));

    qDebug() << "Getting technologies...";
    foreach (ObjectProperties op, ObjectPropertiesList(GetTechnologies()))
    {
        Technology::addTechnology(op.first, op.second);
    }

    Service::mergeServices(ObjectPropertiesList(GetServices()), QList<QDBusObjectPath>());
}


void Manager::onTechnologyAdded(QDBusObjectPath path, QVariantMap properties)
{
    Technology::addTechnology(path, properties);
}

void Manager::onTechnologyRemoved(QDBusObjectPath path)
{
    Technology::removeTechnology(path);
}


void Manager::onServicesChanged(ObjectPropertiesList changed, QList<QDBusObjectPath> removed)
{
    Q_UNUSED(removed);
    Service::mergeServices(changed, removed);
}

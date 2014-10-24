#include "service.h"

QList<ServicePtr>& Service::services()
{
    static QList<ServicePtr> _services;
    return _services;
}

QMap<QString, QVariantMap>& Service::serviceProperties()
{
    static QMap<QString, QVariantMap> _serviceProperties;
    return _serviceProperties;
}

void Service::mergeServices(ObjectPropertiesList changed, QList<QDBusObjectPath> removed)
{
    foreach(QDBusObjectPath path, removed)
    {
        serviceProperties().remove(path.path());
    }

    services().clear();
    foreach (ObjectProperties op, changed)
    {
        QDBusObjectPath path = op.first;
        QVariantMap changedProperties = op.second;

        services().append(ServicePtr(new Service(op.first)));
        foreach (QString key, changedProperties.keys())
        {
            serviceProperties()[path.path()].insert(key, changedProperties[key]);
        }
    }
}

Service::Service(QDBusObjectPath path) : NetConnmanServiceInterface("net.connman", path.path(), QDBusConnection::systemBus()), mPath(path)
{
    connect(this, SIGNAL(PropertyChanged(QString,QDBusVariant)), this, SLOT(onPropertyChange(QString,QDBusVariant)));
}


void Service::onPropertyChange(QString key, QDBusVariant newValue)
{
    qDebug() << mPath.path() << "propertychange:" << key << "->" << newValue.variant();
    serviceProperties()[mPath.path()][key] = newValue.variant();
}

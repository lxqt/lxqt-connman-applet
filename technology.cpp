#include <QDBusConnection>
#include "technology.h"

QList<TechnologyPtr>& Technology::technologies()
{
    static QList<TechnologyPtr> _technologies;
    return _technologies;
}

void Technology::addTechnology(QDBusObjectPath path, QVariantMap properties)
{
    Technology* technology = new Technology(path, properties);
    technology->Scan();
    technologies().append(TechnologyPtr(new Technology(path, properties)));
}


void Technology::removeTechnology(QDBusObjectPath path)
{
    for (int i = 0; i < technologies().size(); i++)
    {
        if (technologies().at(i)->path().path() == path.path())
        {
            technologies().removeAt(i);
            break;
        }
    }
}

Technology::Technology(QDBusObjectPath path, QVariantMap properties) :
    NetConnmanTechnologyInterface("net.connman", path.path(), QDBusConnection::systemBus()), mPath(path), mProperties(properties)
{
    connect(this, SIGNAL(PropertyChanged(QString,QDBusVariant)), this, SLOT(onPropertyChange(QString,QDBusVariant)));
}

void Technology::onPropertyChange(QString key, QDBusVariant newValue)
{
    qDebug() << mPath.path() << "propertyChanged:" << key << "->" << newValue.variant();
    bool shouldScan = key == "Powered" && !mProperties.value(key, false).toBool() && newValue.variant().toBool();
    mProperties.insert(key, newValue.variant());

    if (shouldScan) {
        qDebug() << "Doing scan on" << mPath.path();
        Scan();
    }
}

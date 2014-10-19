#ifndef SERVICE_H
#define SERVICE_H

#include <QSharedPointer>
#include <QVariantMap>

#include "dbus_types.h"
#include "net.connman.Service.h"

class Service;

typedef QSharedPointer<Service> ServicePtr;

class Service : public NetConnmanServiceInterface
{

    Q_OBJECT

public:
    static QList<ServicePtr>& services();
    static void mergeServices(ObjectPropertiesList changed, QList<QDBusObjectPath> removed);

    Service(QDBusObjectPath mPath);

    QDBusObjectPath path() { return mPath;}
    QString state() { return serviceProperties()[mPath.path()].value("State").toString(); }
    QString name() { return serviceProperties()[mPath.path()].value("Name").toString(); }
    QString type() { return serviceProperties()[mPath.path()].value("Type").toString(); }

private slots:
    void onPropertyChange(QString key, QDBusVariant newValue);

private:
    static QMap<QString, QVariantMap>& serviceProperties();
    const QDBusObjectPath mPath;
};


#endif // SERVICE_H

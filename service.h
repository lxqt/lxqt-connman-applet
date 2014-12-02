#ifndef SERVICE_H
#define SERVICE_H

#include <QSharedPointer>
#include <QVariantMap>

#include "dbus_types.h"
#include "net.connman.Service.h"



class Service : public NetConnmanServiceInterface
{
    Q_OBJECT

public:
    Service(QDBusObjectPath mPath);

    QDBusObjectPath path() { return mPath;}
    QString state() { return mProperties.value("State").toString(); }
    QString name() { return mProperties.value("Name").toString(); }
    QString type() { return mProperties.value("Type").toString(); }

    void setProperty(QString key, QVariant newValue);

    bool deleted;

private slots:
    void onPropertyChange(QString key, QDBusVariant newValue);

private:
    const QDBusObjectPath mPath;
    QVariantMap mProperties;
};

QDebug operator<<(QDebug dbg, Service &service);

typedef QList<Service*> ServiceList;


#endif // SERVICE_H

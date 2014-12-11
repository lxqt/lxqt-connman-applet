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
    int signalStrength() { return mProperties.value("Strength").toInt(); }
    QString interfaceName();

    void setProperty(QString key, QVariant newValue);
    QVariant getProperty(QString key);

signals:
    void stateChanged(Service* service);

private slots:
    void onPropertyChange(QString key, QDBusVariant newValue);

private:
    QVariantMap mProperties;
    const QDBusObjectPath mPath;
};

QDebug operator<<(QDebug dbg, Service &service);

typedef QList<Service*> ServiceList;


#endif // SERVICE_H

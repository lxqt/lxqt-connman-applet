#include <QDBusConnection>
#include "technology.h"

void Technology::togglePowered()
{
    bool newState = !powered();
    qDebug() << "Setting power" << newState << "on" << *this;

    QDBusMessage reply = SetProperty("Powered", QDBusVariant(newState)).reply();
    qDebug() << "reply:" << reply.errorMessage();
}


Technology::Technology(QDBusObjectPath path, QVariantMap properties) :
    NetConnmanTechnologyInterface("net.connman", path.path(), QDBusConnection::systemBus()), mPath(path), mProperties(properties)
{
    connect(this, SIGNAL(PropertyChanged(QString,QDBusVariant)), this, SLOT(onPropertyChange(QString,QDBusVariant)));
    Scan();
}

void Technology::onPropertyChange(QString key, QDBusVariant newValue)
{
    qDebug() << "Technology::onPropertyChange" << key << ":" << newValue.variant();
    bool shouldScan = key == "Powered" && !mProperties.value(key, false).toBool() && newValue.variant().toBool();
    mProperties.insert(key, newValue.variant());

    if (shouldScan) {
        Scan();
    }
}


QDebug operator <<(QDebug dbg, Technology &technology)
{
    dbg.nospace() << "Technology(" << technology.path().path() << ")";
    return dbg.maybeSpace();
}

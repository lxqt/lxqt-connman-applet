#ifndef TECHNOLOGY_H
#define TECHNOLOGY_H

#include <QSharedPointer>
#include <QVariantMap>

#include "net.connman.Technology.h"


class Technology : public NetConnmanTechnologyInterface
{
    Q_OBJECT

public:
    Technology(QDBusObjectPath path, QVariantMap properties);

    QDBusObjectPath path() { return mPath; }
    bool powered() { return mProperties.value("Powered").toBool(); }
    bool connected() { return mProperties.value("Connected").toBool(); }
    QString name() { return mProperties.value("Name").toString(); }
    QString type() { return mProperties.value("Type").toString(); }
    void togglePowered();

private slots:
    void onPropertyChange(QString key, QDBusVariant newValue);

private:
    void scanIfNeedBe();
    const QDBusObjectPath mPath;
    QVariantMap mProperties;


};

QDebug operator <<(QDebug dbg, Technology &technology);

typedef QList<Technology*> TechnologyList;

#endif // TECHNOLOGY_H

#ifndef TECHNOLOGY_H
#define TECHNOLOGY_H

#include <QSharedPointer>
#include <QVariantMap>

#include "net.connman.Technology.h"

class Technology;

typedef QSharedPointer<Technology> TechnologyPtr;

class Technology : public NetConnmanTechnologyInterface
{
    Q_OBJECT

public:
    static QList<TechnologyPtr>& technologies();
    static void addTechnology(QDBusObjectPath path, QVariantMap properties);
    static void removeTechnology(QDBusObjectPath path);

    Technology(QDBusObjectPath path, QVariantMap properties);

    QDBusObjectPath path() { return mPath; }
    bool powered() { return mProperties.value("Powered").toBool(); }
    bool connected() { return mProperties.value("Connected").toBool(); }
    QString name() { return mProperties.value("Name").toString(); }
    QString type() { return mProperties.value("Type").toString(); }

private slots:
    void onPropertyChange(QString key, QDBusVariant newValue);

private:
    const QDBusObjectPath mPath;
    QVariantMap mProperties;


};


#endif // TECHNOLOGY_H

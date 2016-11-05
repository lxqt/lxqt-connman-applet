#ifndef ITEM_H
#define ITEM_H

#include <QStandardItem>
#include "dbus_types.h"

class ItemWrapper : public QObject
{
    Q_OBJECT

public:
    enum Role
    {
        WrapperRole = Qt::UserRole,
        OrderRole
    };

    ItemWrapper(QStandardItem* parent, const QString& path, const char* service, const QVariantMap& properties);
    virtual ~ItemWrapper();
    virtual void activate() = 0;
    virtual void update() = 0;
    void setOrder(int order);
    ConnmanObject* connmanObject;
    QStandardItem* item;
private slots:
    virtual void onPropertyChanged();
};

Q_DECLARE_METATYPE(ItemWrapper*)

class ServiceItemWrapper : public ItemWrapper
{
public:
    ServiceItemWrapper(QStandardItem* parent, const QString& path, const QVariantMap& properties);
    virtual ~ServiceItemWrapper() {}
    virtual void activate();
    virtual void update();
};

class TechnologyItemWrapper : public ItemWrapper
{
public:
    TechnologyItemWrapper(QStandardItem* parent, const QString& path, const QVariantMap& properties);
    virtual ~TechnologyItemWrapper() {}
    virtual void activate();
    virtual void update();
};

#endif // ITEM_H

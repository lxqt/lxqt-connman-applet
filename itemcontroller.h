#ifndef ITEM_H
#define ITEM_H

#include <QStandardItem>
#include "dbus_types.h"

class ItemController : public QObject
{
    Q_OBJECT

public:
    enum Role
    {
        WrapperRole = Qt::UserRole,
        OrderRole
    };

    ItemController(QStandardItem* parent, const QString& path, const char* service, const QVariantMap& properties);
    virtual ~ItemController();
    virtual void activate() = 0;
    virtual void update() = 0;
    void setOrder(int order);
    ConnmanObject* connmanObject;
    QStandardItem* item;
private slots:
    virtual void onPropertyChanged();
};

Q_DECLARE_METATYPE(ItemController*)

class ServiceItemController : public ItemController
{
public:
    ServiceItemController(QStandardItem* parent, const QString& path, const QVariantMap& properties);
    virtual ~ServiceItemController() {}
    virtual void activate();
    virtual void update();
};

class TechnologyItemController : public ItemController
{
public:
    TechnologyItemController(QStandardItem* parent, const QString& path, const QVariantMap& properties);
    virtual ~TechnologyItemController() {}
    virtual void activate();
    virtual void update();
};

#endif // ITEM_H

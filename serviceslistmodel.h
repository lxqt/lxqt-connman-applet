#ifndef SERVICESLISTMODEL_H
#define SERVICESLISTMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include "dbus_types.h"

struct ServicesListModelPrivate;

/**
 * @brief The ServicesListModel class wraps a list of services
 */
class ServicesListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ServicesListModel(QObject *parent = 0);
    virtual ~ServicesListModel();
    virtual int rowCount(const QModelIndex& parent) const;
    virtual QVariant data(const QModelIndex& index, int role) const;

public slots:
    void onServicesChanged(ObjectPropertiesList added, const QList<QDBusObjectPath>& removed);
    void onServiceUpdated(const QString& name, const QDBusVariant& newValue);

private:
    ServicesListModelPrivate* modelData;
};

#endif // SERVICESLISTMODEL_H

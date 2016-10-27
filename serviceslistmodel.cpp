#include <QDebug>
#include <QDBusReply>
#include "net.connman.Service.h"
#include "dbus_types.h"
#include "serviceslistmodel.h"

struct ServicesListModelPrivate
{
    QMap<QString, Service*> services;
    QVector<QString> serviceOrder;

};

ServicesListModel::ServicesListModel(QObject* parent) :
    QAbstractListModel(parent),
    modelData(new ServicesListModelPrivate())
{
}

ServicesListModel::~ServicesListModel()
{
    delete modelData;
}

int ServicesListModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : modelData->serviceOrder.size();
}

QVariant ServicesListModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid() && index.row() < modelData->serviceOrder.size() && role == Qt::DisplayRole) {
        QString servicePath = modelData->serviceOrder[index.row()];
        return modelData->services[servicePath]->properties["Name"];
    }
    else {
        return QVariant();
    }
}

void ServicesListModel::onServicesChanged(ObjectPropertiesList added, const QList<QDBusObjectPath>& removed)
{
    qDebug() << "services changed: " << added;
    emit layoutAboutToBeChanged();
    for (QDBusObjectPath path : removed)
    {
        if (modelData->services.contains(path.path()))
        {
            modelData->services.take(path.path())->deleteLater();
        }
    }
    modelData->serviceOrder.clear();
    for (ObjectProperties op: added) {
        modelData->serviceOrder.append(op.first.path());
        if (! modelData->services.contains(op.first.path()))
        {
            Service* service = new Service(op.first.path(), op.second);
            connect(service, SIGNAL(PropertyChanged(const QString&, const QDBusVariant&)),
                             SLOT(onServiceUpdated(const QString&, const QDBusVariant&)));
            modelData->services[op.first.path()] = service;
        }
        modelData->services[op.first.path()]->properties["__position"] = modelData->serviceOrder.size() - 1;
    }
    qDebug() << "services now: ";
    for (const QString& path : modelData->services.keys())
    {
        QVariantMap& properties = modelData->services[path]->properties;
        qDebug() << path << "-> { name: '" << properties["Name"].toString() << "', state:" << properties["State"].toString() << "'}";
    }
    emit layoutChanged();
}

void ServicesListModel::onServiceUpdated(const QString& name, const QDBusVariant& newValue)
{
    qDebug() << "service updated: " << name << ", " << newValue.variant();
    Service* service = dynamic_cast<Service*>(sender());
    service->properties[name] = newValue.variant();
    int row = service->properties["__position"].toInt();
    emit dataChanged(createIndex(row, 0), createIndex(row, 0));
}

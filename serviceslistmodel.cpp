#include <QDebug>
#include <QDBusReply>
#include "dbus_types.h"
#include "serviceslistmodel.h"

ServicesListModel::ServicesListModel(QObject* parent) :
    QAbstractListModel(parent),
    services(),
    serviceOrder()
{
}

ServicesListModel::~ServicesListModel()
{
}

int ServicesListModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : serviceOrder.size();
}

QVariant ServicesListModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid() && index.row() < serviceOrder.size()) {
        const ConnmanObject& service = *services[serviceOrder[index.row()]];
        switch(role) {
        case Qt::DisplayRole:
            return service["Name"];
        case Qt::DecorationRole:
            if (service["Type"] == "wifi") {
                return IconProducer::instance().wireless(service["Strength"].toInt());
            }
            else {
                // TODO: This means that everything else than wifi is depicted as wired. We should
                // do something for bluetooth...
                return service["State"] == "offline" ? IconProducer::instance().disconnected() :
                                                       IconProducer::instance().wiredConnected();
            }
        default:
            return QVariant();
        }

    }
    else {
        return QVariant();
    }
}

void ServicesListModel::onServicesChanged(ObjectPropertiesList added, const QList<QDBusObjectPath>& removed)
{
    emit layoutAboutToBeChanged();
    for (QDBusObjectPath path : removed)
    {
        if (services.contains(path.path()))
        {
            services.take(path.path())->deleteLater();
        }
    }
    serviceOrder.clear();
    for (ObjectProperties op: added) {
        QString path = op.first.path();
        const QVariantMap& properties = op.second;
        serviceOrder.append(path);
        ConnmanObject*& service = services[path];
        if (service == 0) {
            service = new ConnmanObject(op.first.path(), "net.connman.Service");
            connect(service, SIGNAL(PropertyChanged(const QString&, const QVariant&)),
                             SLOT(onServiceUpdated(const QString&, const QVariant&)));
        }
        for (const QString& key : properties.keys()) {
            (*service)[key] = properties[key];
        }
        (*services[op.first.path()])["__position"] = serviceOrder.size() - 1;
    }
    emit layoutChanged();
}

void ServicesListModel::onServiceUpdated(const QString& name, const QVariant& newValue)
{
    qDebug() << "service updated: " << name << ", " << newValue;
    ConnmanObject& service = *dynamic_cast<ConnmanObject*>(sender());
    service[name] = newValue;
    int row = service["__position"].toInt();
    emit dataChanged(createIndex(row, 0), createIndex(row, 0));
}

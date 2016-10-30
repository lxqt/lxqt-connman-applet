#include <QDebug>
#include <QDBusReply>
#include <QFont>
#include "dbus_types.h"
#include "iconproducer.h"
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
            if (service["State"] == "online") {
                return service["Name"].toString() + " " + QChar(0x2713);
            }
            else {
                return  service["Name"].toString();
            }
        case Qt::FontRole:
            if (service["State"] == "ready" || service["State"] == "online") {
                QFont font;
                font.setBold(true);
                return font;
            }
            else {
                return QVariant();
            }
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
            connect(service, SIGNAL(PropertyChanged(const QString&, const QDBusVariant&)),
                             SLOT(onServiceUpdated(const QString&, const QDBusVariant&)));
            for (const QString& key : properties.keys()) {
                (*service)[key] = properties[key];
            }
            if (service->contains("Name")) {
                emit serviceNamed(path, (*service)["Name"].toString());
            }
         }
         (*services[op.first.path()])["__position"] = serviceOrder.size() - 1;
    }
    emit layoutChanged();
}

void ServicesListModel::onServiceUpdated(const QString& name, const QDBusVariant& newValue)
{
    ConnmanObject& service = *dynamic_cast<ConnmanObject*>(sender());
    service[name] = newValue.variant();
    if (name == "Name") {
        emit serviceNamed(name, newValue.variant().toString());
    }
    int row = service["__position"].toInt();
    emit dataChanged(createIndex(row, 0), createIndex(row, 0));
}

void ServicesListModel::onServiceActivated(const QModelIndex& index)
{
    if (index.isValid() && index.row() < serviceOrder.size()) {
        ConnmanObject& service = *services[serviceOrder[index.row()]];
        QString state = service["State"].toString();
        if (state == "idle" || state == "failure") {
            service.asyncCall("Connect");
        }
        else if (state == "association" || state == "configuration" || state == "ready" || state == "online") {
            service.asyncCall("Disconnect");
        }
    }
}

#include <QDBusPendingCall>
#include "iconproducer.h"
#include "itemcontroller.h"

ItemController::ItemController(QStandardItem* parent, const QString& path, const char* service, const QVariantMap& properties):
    QObject(),
    connmanObject(new ConnmanObject(path, service, properties)),
    item(new QStandardItem())
{
    connect(connmanObject, &ConnmanObject::PropertyChanged, this, &ItemController::onPropertyChanged);
    parent->appendRow(item);
    item->setData(QVariant::fromValue(this), WrapperRole);
    item->setEditable(false);
}

ItemController::~ItemController()
{
    item->parent()->removeRow(item->row());
    connmanObject->deleteLater();
}

void ItemController::setOrder(int order)
{
   item->setData(order, OrderRole);
}

void ItemController::onPropertyChanged()
{
    update();
}


ServiceItemController::ServiceItemController(QStandardItem* parent, const QString& path, const QVariantMap& properties) :
    ItemController(parent, path, "net.connman.Service", properties)
{
}

void ServiceItemController::update()
{
    QString state = connmanObject->properties["State"].toString();
    QString type = connmanObject->properties["Type"].toString();
    QString displayData = connmanObject->properties["Name"].toString();
    int strength = connmanObject->properties["Strength"].toInt();

    item->setData(state == "online" ? displayData + ' ' + QChar(0x2713) : displayData, Qt::DisplayRole);
    item->setData(state == "ready" || state == "online" ? QFont("", -1, QFont::Bold) : QVariant(), Qt::FontRole);

    if (type == "wifi") {
        item->setData(IconProducer::instance().wireless(strength), Qt::DecorationRole);
    }
    else if (state == "offline") {
        item->setData(IconProducer::instance().disconnected(), Qt::DecorationRole);
    }
    else {
        item->setData(IconProducer::instance().wiredConnected(), Qt::DecorationRole);
    }
}

void ServiceItemController::activate()
{
    QString state = connmanObject->properties["State"].toString();
    if (state == "idle" || state == "failure") {
        connmanObject->asyncCall("Connect");
    }
    else if (state == "association" || state == "configuration" || state == "ready" || state == "online") {
        connmanObject->asyncCall("Disconnect");
    }
}

TechnologyItemController::TechnologyItemController(QStandardItem* parent, const QString& path, const QVariantMap& properties) :
    ItemController(parent, path, "net.connman.Technology", properties)
{
}

void TechnologyItemController::update()
{
    QString type = connmanObject->properties["Type"].toString();
    bool powered = connmanObject->properties["Powered"].toBool();
    QString name = connmanObject->properties["Name"].toString();

    item->setData(name, Qt::DisplayRole);
    if (type == "ethernet") {
        item->setData(IconProducer::instance().wiredConnected(), Qt::DecorationRole);
    }
    else if (type == "wifi") {
        item->setData(IconProducer::instance().wireless(80), Qt::DecorationRole);
    }
    else {
        item->setData(IconProducer::instance().blanc(), Qt::DecorationRole);
    }
    item->setData(powered ? QVariant() : QFont("", -1, -1, true), Qt::FontRole);
    item->setData(powered ? QVariant() : QColor("lightgrey"), Qt::ForegroundRole);
}

void TechnologyItemController::activate()
{
    bool newPowered = !(connmanObject->properties["Powered"].toBool());
    connmanObject->asyncCall("SetProperty", QVariant("Powered"), QVariant::fromValue(QDBusVariant(newPowered)));
}




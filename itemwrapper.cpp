#include <QDBusPendingCall>
#include "iconproducer.h"
#include "itemwrapper.h"

ItemWrapper::ItemWrapper(QStandardItem* parent, const QString& path, const char* service, const QVariantMap& properties):
    QObject(),
    connmanObject(new ConnmanObject(path, service, properties)),
    item(new QStandardItem())
{
    connect(connmanObject, &ConnmanObject::PropertyChanged, this, &ItemWrapper::onPropertyChanged);
    parent->appendRow(item);
    item->setData(QVariant::fromValue(this), WrapperRole);
    item->setEditable(false);
}

ItemWrapper::~ItemWrapper()
{
    item->parent()->removeRow(item->row());
    connmanObject->deleteLater();
}

void ItemWrapper::setOrder(int order)
{
   item->setData(order, OrderRole);
}

void ItemWrapper::onPropertyChanged()
{
    update();
}


ServiceItemWrapper::ServiceItemWrapper(QStandardItem* parent, const QString& path, const QVariantMap& properties) :
    ItemWrapper(parent, path, "net.connman.Service", properties)
{
}

void ServiceItemWrapper::update()
{
    QString state = connmanObject->value("State").toString();
    QString type = connmanObject->value("Type").toString();
    QString displayData = connmanObject->value("Name").toString();
    int strength = connmanObject->value("Strength").toInt();

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

void ServiceItemWrapper::activate()
{
    QString state = connmanObject->value("State").toString();
    if (state == "idle" || state == "failure") {
        connmanObject->asyncCall("Connect");
    }
    else if (state == "association" || state == "configuration" || state == "ready" || state == "online") {
        connmanObject->asyncCall("Disconnect");
    }
}

TechnologyItemWrapper::TechnologyItemWrapper(QStandardItem* parent, const QString& path, const QVariantMap& properties) :
    ItemWrapper(parent, path, "net.connman.Technology", properties)
{
}

void TechnologyItemWrapper::update()
{
    QString type = connmanObject->value("Type").toString();
    bool powered = connmanObject->value("Powered").toBool();
    QString name = connmanObject->value("Name").toString();

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

void TechnologyItemWrapper::activate()
{
    bool newPowered = !(connmanObject->value("Powered").toBool());
    connmanObject->asyncCall("SetProperty", QVariant("Powered"), QVariant::fromValue(QDBusVariant(newPowered)));
}




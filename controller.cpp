#include <QDBusReply>
#include <QMenu>
#include <QMessageBox>
#include "iconproducer.h"
#include "agentadaptor.h"
#include "dbus_types.h"
#include "controller.h"


Controller::Controller() :
    manager(),
    agent(),
    model(),
    servicesWindow(),
    trayIcon(),
    connectionTypesItem("Connection types"),
    servicesItem("Services")
{
    QDBusObjectPath agentPath("/org/lxqt/lxqt_connman_agent");
    new AgentAdaptor(&agent);
    QDBusConnection::systemBus().registerObject(agentPath.path(), &agent);
    manager.call("RegisterAgent", QVariant::fromValue(agentPath));

    model.insertRow(0, &connectionTypesItem);
    model.insertRow(1, &servicesItem);
    model.setSortRole(Qt::UserRole + 1);
    connect(&manager,
            SIGNAL(TechnologyAdded(const QDBusObjectPath&, const QVariantMap&)),
            SLOT(onTechnologyAdded(const QDBusObjectPath&, const QVariantMap&)));

    connect(&manager,
            SIGNAL(TechnologyRemoved(const QDBusObjectPath&)),
            SLOT(onTechnologyRemoved(const QDBusObjectPath&)));

    connect(&manager,
            SIGNAL(ServicesChanged(ObjectPropertiesList, const QList<QDBusObjectPath>&)),
            SLOT(onServicesUpdated(const ObjectPropertiesList&,  const QList<QDBusObjectPath>&)));

    connect(&manager, SIGNAL(PropertyChanged(const QString&, const QDBusVariant&)), SLOT(updateTrayIcon()));
    connect(&IconProducer::instance(), SIGNAL(iconsChanged()), &model, SIGNAL(layoutChanged()));
    connect(&IconProducer::instance(), SIGNAL(iconsChanged()), SLOT(updateTrayIcon()));

    connect(&servicesWindow, SIGNAL(activated(const QModelIndex&)), SLOT(onItemActivated(const QModelIndex&)));

    QDBusReply<ObjectPropertiesList> GetTechnologiesReply = manager.call("GetTechnologies");;
    for (ObjectProperties op : GetTechnologiesReply.value()) {
        onTechnologyAdded(op.first, op.second);
    }

    QDBusReply<ObjectPropertiesList> GetServicesReply = manager.call("GetServices");
    onServicesUpdated(GetServicesReply.value(), QList<QDBusObjectPath>());

    servicesWindow.setModel(&model);

    QMenu *menu = new QMenu();
    menu->addAction(tr("Services..."), &servicesWindow, SLOT(show()));
    menu->addAction(QIcon::fromTheme("help-about"), tr("About"), this, SLOT(about()));
    menu->addAction(QIcon::fromTheme("application-exit"), tr("Quit"), qApp, SLOT(quit()));
    connect(&trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                       SLOT(onTrayIconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon.setContextMenu(menu);
    updateTrayIcon();
    trayIcon.show();
  }

void Controller::updateTechnologyPresentationData(QStandardItem* item)
{
    ConnmanObject& technology = *connmanObject(item);
    QString type = technology["Type"].toString();
    bool powered = technology["Powered"].toBool();
    QString name = technology["Name"].toString();

    item->setData(name, Qt::DisplayRole);

    QIcon icon;
    if (type == "ethernet") {
        icon = IconProducer::instance().wiredConnected();
    }
    else if (type == "wifi") {
        icon = IconProducer::instance().wireless(80);
    }
    else {
        icon = IconProducer::instance().blanc();
    }
    item->setData(icon, Qt::DecorationRole);

    QFont font;
    QColor color;
    if (! powered) {
        font.setItalic(true);
        color = QColor("lightgrey");
    }
    item->setData(font, Qt::FontRole);
    item->setData(color, Qt::ForegroundRole);

}

void Controller::updateServicePresentationData(QStandardItem* item)
{
    ConnmanObject& service = *connmanObject(item);
    QString state = service["State"].toString();
    QString type = service["Type"].toString();
    QString displayData = service["Name"].toString();
    int strength = service["Strength"].toInt();

    if (state == "online") {
        displayData = displayData + " " + QChar(0x2713);
    }
    else if  (state != "idle" && state != "ready"){
        displayData = displayData + " (" + state + ")";
    }
    item->setData(displayData, Qt::DisplayRole);

    QFont font;
    if (state == "ready" || state == "online") {
        font.setBold(true);
    }
    item->setData(font, Qt::FontRole);

    QIcon icon;
    if (type == "wifi") {
        icon = IconProducer::instance().wireless(strength);
    }
    else {
        // TODO: This means that everything else than wifi is depicted as wired. We should
        // do something for bluetooth, p2p ..
        icon =  state == "offline" ? IconProducer::instance().disconnected() : IconProducer::instance().wiredConnected();
    }
    item->setData(icon, Qt::DecorationRole);

    agent.setEntityName(service.path(), service["Name"].toString());
}

ConnmanObject* Controller::connmanObject(QStandardItem* item)
{
    return item->data(Qt::UserRole).value<ConnmanObject*>();
}

void Controller::remove(const QString& path)
{
    QStandardItem *item = items.take(path);
    connmanObject(item)->deleteLater();
    item->parent()->removeRow(item->row());
}


void Controller::onTechnologyAdded(const QDBusObjectPath& path, const QVariantMap& properties)
{
    QStandardItem* item = new QStandardItem();
    ConnmanObject* technology = new ConnmanObject(path.path(), "net.connman.Technology", properties);
    connect(technology, SIGNAL(PropertyChanged(const QString&, const QDBusVariant&)),
                        SLOT(onTechnologyPropertyChanged(const QString&, const QDBusVariant&)));
    item->setData(QVariant::fromValue(technology), Qt::UserRole);
    updateTechnologyPresentationData(item);
    item->setEditable(false);
    items[path.path()] = item;
    connectionTypesItem.appendRow(QList<QStandardItem*>({item}));
}


void Controller::onTechnologyRemoved(const QDBusObjectPath& path)
{
    remove(path.path());
}

void Controller::onServicesUpdated(ObjectPropertiesList services, const QList<QDBusObjectPath>& removed)
{
    for (const auto& path : removed) {
        remove(path.path());
    }

    QList<QStandardItem*> newItems;
    int count = 0;
    for (const auto& op : services) {
        count++;
        QString path = op.first.path();
        QVariantMap properties = op.second;

        if (!items.contains(path)) {
            if (properties.contains("Name") && !properties["Name"].toString().isEmpty()) {
                ConnmanObject* service = new ConnmanObject(path, "net.connman.Service", properties);
                connect(service, SIGNAL(PropertyChanged(const QString&, const QDBusVariant&)),
                                 SLOT(onServicePropertyChanged(const QString&, const QDBusVariant&)));
                items[path] = new QStandardItem();
                items[path]->setData(QVariant::fromValue(service), Qt::UserRole);
                items[path]->setEditable(false);
                updateServicePresentationData(items[path]);
                newItems.append(items[path]);
            }
            else {
                // This would be a hidden service. We leave them out as we don't (yet) have
                // functionality to handle them
                continue;
            }
        }
        items[path]->setData(count, Qt::UserRole + 1);
    }

    if (newItems.size() > 0) {
        servicesItem.appendRows(newItems);
    }

    servicesItem.sortChildren(0);
}

void Controller::onTechnologyPropertyChanged(const QString& name, const QDBusVariant& newValue)
{
    QString path = dynamic_cast<ConnmanObject*>(sender())->path();
    if (items.contains(path)) {
        connmanObject(items[path])->insert(name, newValue.variant());
        updateTechnologyPresentationData(items[path]);
    }
}

void Controller::onServicePropertyChanged(const QString& name, const QDBusVariant& newValue)
{
    QString path = dynamic_cast<ConnmanObject*>(sender())->path();
    if (items.contains(path)) {
        connmanObject(items[path])->insert(name, newValue.variant());
        updateServicePresentationData(items[path]);
    }
}

void Controller::onItemActivated(const QModelIndex& index)
{
    ConnmanObject* connmanObject = index.data(Qt::UserRole).value<ConnmanObject*>();
    if (connmanObject != 0) {
        if (connmanObject->path().startsWith("/net/connman/technology")) { // TODO Can we trust this?
            bool newPowered = !(connmanObject->value("Powered").toBool());
            connmanObject->asyncCall("SetProperty", QVariant("Powered"), QVariant::fromValue(QDBusVariant(newPowered)));
        }
        else if (connmanObject->path().startsWith("/net/connman/service")) { // ..do
            QString state = connmanObject->value("State").toString();
            if (state == "idle" || state == "failure") {
                connmanObject->asyncCall("Connect");
            }
            else if (state == "association" || state == "configuration" || state == "ready" || state == "online") {
                connmanObject->asyncCall("Disconnect");
            }
        }
    }
}

void Controller::updateTrayIcon()
{
    for (QStandardItem* item : items.values()) {
        ConnmanObject* obj = connmanObject(item);
        if (obj->path().startsWith("/net/connman/service")) {
            QString state = obj->value("State").toString();
            int signalStrength = obj->value("Strength").toInt();

            if (state == "ready" || state == "online") {
                if (signalStrength > 0) {
                    trayIcon.setIcon(IconProducer::instance().wireless(signalStrength));
                }
                else {
                    trayIcon.setIcon(IconProducer::instance().wiredConnected());
                }
                return;
            }
        }
    }

    trayIcon.setIcon(IconProducer::instance().disconnected());

}

void Controller::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    servicesWindow.show();
}

void Controller::about()
{
    QMessageBox::about(0,
                       tr("About"),
                       tr( "<p>"
                           "  <b>LXQt Connman Client</b>"
                           "</p>"
                           "<p>"
                           "Copyright 2014, 2015, 2016"
                           "</p>"
                           "<p>"
                           "Christian Surlykke"
                           "</p>"
                           ));

}

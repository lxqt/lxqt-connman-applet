#include <QDBusReply>
#include <QMenu>
#include <QMessageBox>
#include "iconproducer.h"
#include "agentadaptor.h"
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
    model.setSortRole(ItemController::OrderRole);
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

void Controller::onTechnologyAdded(const QDBusObjectPath& path, const QVariantMap& properties)
{
    if (technologyItemWrappers.contains(path.path())) {
        return;
    }
    TechnologyItemController* item = new TechnologyItemController(&connectionTypesItem, path.path(), properties);
    technologyItemWrappers[path.path()] = item;
    technologyItemWrappers[path.path()]->update();
    servicesWindow.expandAll();
}


void Controller::onTechnologyRemoved(const QDBusObjectPath& path)
{
    if (technologyItemWrappers.contains(path.path())) {
        technologyItemWrappers.take(path.path())->deleteLater();
    }
}

void Controller::onServicesUpdated(ObjectPropertiesList services, const QList<QDBusObjectPath>& removed)
{
    for (const auto& path : removed) {
        if (serviceItemWrappers.contains(path.path())) {
            serviceItemWrappers.take(path.path())->deleteLater();
        }
    }

    int count = 0;
    for (const auto& op : services) {
        QString path = op.first.path();
        QVariantMap properties = op.second;

        if (!serviceItemWrappers.contains(path)) {
            if (properties.contains("Name") && !properties["Name"].toString().isEmpty()) {
                serviceItemWrappers[path] = new ServiceItemController(&servicesItem, path, properties);
                serviceItemWrappers[path]->update();
            }
            else {
                // This would be a hidden essid. We leave them out as we don't (yet) have
                // functionality to handle them
                continue;
            }
        }
        serviceItemWrappers[path]->setOrder(count++);
    }

    servicesItem.sortChildren(0);
    servicesWindow.expandAll();
}

void Controller::onItemActivated(const QModelIndex& index)
{
    ItemController *wrapper = model.itemFromIndex(index)->data(ItemController::WrapperRole).value<ItemController*>();
    if (wrapper) {
        wrapper->activate();
    }
}

void Controller::updateTrayIcon()
{
    for (ServiceItemController* item : serviceItemWrappers.values()) {
        QString state = item->connmanObject->properties["State"].toString();
        int signalStrength = item->connmanObject->properties["Strength"].toInt();

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

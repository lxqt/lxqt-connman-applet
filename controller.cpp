#include "net.connman.Manager.h"
#include "controller.h"

Controller::Controller() :
    manager("net.connman", "/", QDBusConnection::systemBus()),
    technologiesListModel(),
    servicesListModel(),
    servicesWindow(&technologiesListModel, &servicesListModel),
    trayIcon()
{
    void ServicesChanged(ObjectPropertiesList changed, const QList<QDBusObjectPath> &removed);
    void TechnologyAdded(const QDBusObjectPath &path, const QVariantMap &properties);
    void TechnologyRemoved(const QDBusObjectPath &path);


    connect(&manager, SIGNAL(TechnologyAdded(const QDBusObjectPath&, const QVariantMap&)),
            &technologiesListModel, SLOT(onTechnologyAdded(const QDBusObjectPath&, const QVariantMap&)));

    connect(&manager, SIGNAL(TechnologyRemoved(const QDBusObjectPath&)),
            &technologiesListModel, SLOT(onTechnologyRemoved(const QDBusObjectPath&)));

    connect(&manager,
            SIGNAL(ServicesChanged(ObjectPropertiesList, const QList<QDBusObjectPath>&)),
            &servicesListModel,
            SLOT(onServicesChanged(const ObjectPropertiesList&,  const QList<QDBusObjectPath>&)));

    QDBusReply<ObjectPropertiesList> GetTechnologiesReply = manager.GetTechnologies();
    for (ObjectProperties op : GetTechnologiesReply.value()) {
        technologiesListModel.onTechnologyAdded(op.first, op.second);
    }

    QDBusReply<ObjectPropertiesList> GetServicesReply = manager.GetServices();
    servicesListModel.onServicesChanged(GetServicesReply.value(), QList<QDBusObjectPath>());

    servicesWindow.show();
}

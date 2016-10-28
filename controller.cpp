#include <QDBusReply>
#include "controller.h"

Controller::Controller() :
    manager(),
    technologiesListModel(),
    servicesListModel(),
    servicesWindow(&technologiesListModel, &servicesListModel),
    trayIcon()
{
    connect(&manager, SIGNAL(TechnologyAdded(const QDBusObjectPath&, const QVariantMap&)),
            &technologiesListModel, SLOT(onTechnologyAdded(const QDBusObjectPath&, const QVariantMap&)));

    connect(&manager, SIGNAL(TechnologyRemoved(const QDBusObjectPath&)),
            &technologiesListModel, SLOT(onTechnologyRemoved(const QDBusObjectPath&)));

    connect(&manager,
            SIGNAL(ServicesChanged(ObjectPropertiesList, const QList<QDBusObjectPath>&)),
            &servicesListModel,
            SLOT(onServicesChanged(const ObjectPropertiesList&,  const QList<QDBusObjectPath>&)));

    QDBusReply<ObjectPropertiesList> GetTechnologiesReply = manager.call("GetTechnologies");;
    for (ObjectProperties op : GetTechnologiesReply.value()) {
        technologiesListModel.onTechnologyAdded(op.first, op.second);
    }

    QDBusReply<ObjectPropertiesList> GetServicesReply = manager.call("GetServices");
    servicesListModel.onServicesChanged(GetServicesReply.value(), QList<QDBusObjectPath>());

    servicesWindow.show();
}

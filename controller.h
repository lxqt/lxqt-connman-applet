#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QApplication>
#include <QSystemTrayIcon>
#include <QStandardItemModel>
#include <QSharedPointer>
#include "dbus_types.h"
#include "serviceswindow.h"
#include "itemwrapper.h"
#include "agent.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller();

private:
    ConnmanManager manager;
    Agent agent;
    QStandardItemModel model;
    ServicesWindow servicesWindow;
    QSystemTrayIcon trayIcon;

    QStandardItem connectionTypesItem;
    QStandardItem servicesItem;
    QMap<QString, TechnologyItemWrapper*> technologyItemWrappers;
    QMap<QString, ServiceItemWrapper*> serviceItemWrappers;

private slots:
    void onTechnologyAdded(const QDBusObjectPath& path, const QVariantMap& properties);
    void onTechnologyRemoved(const QDBusObjectPath& path);
    void onServicesUpdated(ObjectPropertiesList services, const QList<QDBusObjectPath>& remove);
    void onItemActivated(const QModelIndex& index);
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void updateTrayIcon();
    void about();
};

#endif // CONTROLLER_H

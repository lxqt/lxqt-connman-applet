#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QApplication>
#include <QSystemTrayIcon>
#include <QStandardItemModel>
#include <QSharedPointer>
#include "connmanobject.h"
#include "serviceswindow.h"
#include "itemcontroller.h"
#include "agent.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller();

private:
    QStandardItemModel model;
    QStandardItem* technologyItemsParent;
    QStandardItem* serviceItemsParent;
    QMap<QString, TechnologyItemController*> technologyItemControllers;
    QMap<QString, ServiceItemController*> serviceItemControllers;

    ConnmanManager manager;
    Agent agent;
    ServicesWindow servicesWindow;
    QSystemTrayIcon trayIcon;

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

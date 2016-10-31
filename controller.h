#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QApplication>
#include <QSystemTrayIcon>
#include <QStandardItemModel>
#include "dbus_types.h"
#include "serviceswindow.h"
#include "agent.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller();

private:
    void updateTechnologyPresentationData(QStandardItem* item);
    void updateServicePresentationData(QStandardItem* item);
    ConnmanObject* connmanObject(QStandardItem* item);
    void remove(const QString& path);

    ConnmanManager manager;
    Agent agent;
    QStandardItemModel model;
    ServicesWindow servicesWindow;
    QSystemTrayIcon trayIcon;

    QStandardItem connectionTypesItem;
    QStandardItem servicesItem;
    QMap<QString, QStandardItem*> items;

private slots:
    void onTechnologyAdded(const QDBusObjectPath& path, const QVariantMap& properties);
    void onTechnologyRemoved(const QDBusObjectPath& path);
    void onServicesUpdated(ObjectPropertiesList services, const QList<QDBusObjectPath>& remove);
    void onTechnologyPropertyChanged(const QString& name, const QDBusVariant& newValue);
    void onServicePropertyChanged(const QString& name, const QDBusVariant& newValue);
    void onItemActivated(const QModelIndex& index);
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void updateTrayIcon();
    void about();
};

#endif // CONTROLLER_H

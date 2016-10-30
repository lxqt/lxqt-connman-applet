#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QApplication>
#include <QSystemTrayIcon>
#include <QStandardItemModel>
//#include "technologieslistmodel.h"
//#include "serviceslistmodel.h"
#include "dbus_types.h"
#include "serviceswindow.h"
#include "agent.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller();

private:
    ConnmanManager manager;
    //TechnologiesListModel technologiesListModel;
    //ServicesListModel servicesListModel;
    Agent agent;
    QStandardItemModel model;
    ServicesWindow servicesWindow;
    QSystemTrayIcon trayIcon;

    QStandardItem connectionTypesItem;
    QStandardItem servicesItem;
    QMap<QString, QStandardItem*> items;

    void setTechnologyData(QStandardItem* item);
    void setServiceData(QStandardItem* item);
    ConnmanObject* connmanObject(QStandardItem* item);
    void remove(const QString& path);

private slots:
    void addTechnology(const QDBusObjectPath& path, const QVariantMap& properties);
    void removeTechnology(const QDBusObjectPath& path);
    void updateServices(ObjectPropertiesList services, const QList<QDBusObjectPath>& remove);
    void updateTechnology(const QString& name, const QDBusVariant& newValue);
    void updateService(const QString& name, const QDBusVariant& newValue);
    void activateItem(const QModelIndex& index);
};

#endif // CONTROLLER_H

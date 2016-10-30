#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QApplication>
#include <QSystemTrayIcon>
#include "dbus_types.h"
#include "technologieslistmodel.h"
#include "serviceslistmodel.h"
#include "serviceswindow.h"
#include "agent.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller();
private:
    ConnmanManager manager;
    TechnologiesListModel technologiesListModel;
    ServicesListModel servicesListModel;
    Agent agent;
    ServicesWindow servicesWindow;
    QSystemTrayIcon trayIcon;
};

#endif // CONTROLLER_H

#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QSystemTrayIcon>
#include "net.connman.Manager.h"
#include "dbus_types.h"

class SystemTray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit SystemTray(QObject *parent = 0);

private:
    void registerDBusTypes();
    void buildMenu();
    void addTechnologies();
    void addServices();
};

#endif // SYSTEMTRAY_H

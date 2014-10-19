#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QSystemTrayIcon>
#include <QActionGroup>
#include <QList>

#include "dbus_types.h"

#include "net.connman.Manager.h"
#include "net.connman.Service.h"
#include "net.connman.Technology.h"


class SystemTray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit SystemTray(QObject *parent = 0);

private:

    QActionGroup technologyEntries;
    QActionGroup serviceEntries;

private slots:
    void buildMenu();
    void onTechnologyClicked(QAction* action);
    void onServiceClicked(QAction* action);
};

#endif // SYSTEMTRAY_H

#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QSystemTrayIcon>

class SystemTray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit SystemTray(QObject *parent = 0);

private:
    void buildMenu();
    void addTechnologies();
    void addServices();
};

#endif // SYSTEMTRAY_H

#ifndef CONNECTIONSTATE_H
#define CONNECTIONSTATE_H

#include "service.h"

class ConnectionState : public QObject
{

    Q_OBJECT

public:
    static ConnectionState *instance();

    Service *connectedWireless() { return connectedOfType("wifi"); }
    Service *connectedEthernet() { return connectedOfType("ethernet"); }
    Service *connectedBluetooth() { return connectedOfType("bluetooth"); }

signals:
    void changed();

private:
    Service *connectedOfType(QString type);
    ConnectionState() {}
};

#endif // CONNECTIONSTATE_H

#include "connectionstate.h"
#include "manager.h"
#include "service.h"

ConnectionState* ConnectionState::instance()
{
    static ConnectionState *_instance = new ConnectionState();
    return _instance;
}


Service *ConnectionState::connectedOfType(QString type)
{
    foreach (Service *service, Manager::instance().services())
    {
        if (service->type() == type)
        {
            return service;
        }
    }

    return 0;
}

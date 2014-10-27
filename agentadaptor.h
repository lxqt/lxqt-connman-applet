#ifndef AGENTADAPTER_H
#define AGENTADAPTER_H

#include <QDBusAbstractAdaptor>

class AgentAdaptor : public QDBusAbstractAdaptor
{
public:
    AgentAdaptor(QObject *parent);
};

#endif // AGENTADAPTER_H

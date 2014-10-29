#ifndef AGENT_H
#define AGENT_H

#include <QDBusObjectPath>

class Agent : public QObject
{
    Q_OBJECT
public:
    explicit Agent();

private:
    QDBusObjectPath path;
};

#endif // AGENT_H

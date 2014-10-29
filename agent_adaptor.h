#ifndef AGENTADAPTER_H
#define AGENTADAPTER_H

#include <QDBusAbstractAdaptor>
#include <QDBusObjectPath>
#include <QVariantMap>
#include "agent.h"

class AgentAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "net.connman.Agent")
public:
    AgentAdaptor(Agent *agent);

public slots:

    QString ReportError(QDBusObjectPath service, QString errorMsg);
    QString ReportPeerError(QDBusObjectPath peer, QString errorMsg);
    void RequestBrowser(QDBusObjectPath service, QString url);
    QVariantMap RequestInput(QDBusObjectPath service, QVariantMap fields);
    void Cancel();

private:
    Agent* agent;
};

#endif // AGENTADAPTER_H

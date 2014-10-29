#include "agent_adaptor.h"
#include <QDebug>

AgentAdaptor::AgentAdaptor(Agent *agent) : agent(agent), QDBusAbstractAdaptor(agent)
{
}


QString AgentAdaptor::ReportError(QDBusObjectPath service, QString errorMsg) {
    qDebug() << "ReportError, service:" << service.path() << ", errorMsg:" << errorMsg;
}

QString AgentAdaptor::ReportPeerError(QDBusObjectPath peer, QString errorMsg) {
    qDebug() << "ReportPeerError, service:" << peer.path() << ", errorMsg:" << errorMsg;
}

void AgentAdaptor::RequestBrowser(QDBusObjectPath service, QString url) {
    qDebug() << "RequestBrowser, service:" << service.path() << ", url:" << url;
}

QVariantMap AgentAdaptor::RequestInput(QDBusObjectPath service, QVariantMap fields) {
    qDebug() << "service:" << service.path();
    foreach (QString key, fields.keys()) {
        qDebug() << key << "->" << fields[key];
    }
    qDebug() << "-----------------------------";
}

void AgentAdaptor::Cancel() {
    qDebug() << "Cancel";
}

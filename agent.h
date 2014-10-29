#ifndef AGENT_H
#define AGENT_H

#include <QDBusObjectPath>

class Agent : public QObject
{
    Q_OBJECT
public:
    explicit Agent();

    void Release() {}
    void Cancel() {}
    void ReportError(QDBusObjectPath service, QString errorMessage){}
    void ReportPeerError(QDBusObjectPath peer, QString errorMessage){}
    void RequestBrowser(QDBusObjectPath service, QString url){}
    QVariantMap RequestInput(QDBusObjectPath service, QVariantMap fields) { return QVariantMap();}
    QVariantMap RequestPeerAuthorization(QDBusObjectPath peer, QVariantMap fields) { return QVariantMap(); }
private:
    QDBusObjectPath path;
};

#endif // AGENT_H

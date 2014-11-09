#ifndef AGENT_H
#define AGENT_H

#include <QDBusContext>
#include <QDBusObjectPath>
#include <QDebug>

class Agent : public QObject, protected QDBusContext
{
    Q_OBJECT
public:
    explicit Agent();

    void Release();
    void Cancel();
    void ReportError(QDBusObjectPath service, QString errorMessage);
    void ReportPeerError(QDBusObjectPath peer, QString errorMessage);
    void RequestBrowser(QDBusObjectPath service, QString url);
    QVariantMap RequestInput(QDBusObjectPath service, QVariantMap fields);
    QVariantMap RequestPeerAuthorization(QDBusObjectPath peer, QVariantMap fields);
    QString Introspect();

signals:
    void operationCanceled();

private:
    QDBusObjectPath path;
};

#endif // AGENT_H

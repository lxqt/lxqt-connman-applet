#ifndef SERVICEFRAME_H
#define SERVICEFRAME_H

#include <QVariantMap>
#include <QTimer>
#include "clickableframe.h"
#include "net.connman.Service.h"

namespace Ui {
class ServiceFrame;
}

class ServiceFrame : public ClickableFrame
{
    Q_OBJECT

public:
    explicit ServiceFrame(QString path, const QVariantMap& properties, QWidget *parent = 0);
    ~ServiceFrame();

    virtual void click();
    bool connected();
    int signalStrength();

signals:
    void stateChanged();

private slots:
    void onPropertyChanged(const QString& name, const QDBusVariant& value);
    void onErrorTimeout();

private:
    void updateUI();

    QString type() { return properties["Type"].toString(); }
    QString state() { return properties["State"].toString(); }

    static const QString stylesheetWifiReady;
    static const QString stylesheetWifiOnline;
    static const QString stylesheetOtherOnline;

    Ui::ServiceFrame *ui;
    QVariantMap properties;
    NetConnmanServiceInterface serviceInterface;
};

#endif // SERVICEFRAME_H

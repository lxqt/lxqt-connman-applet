#ifndef ICONPRODUCER_H
#define ICONPRODUCER_H
#include <QIcon>

class IconProducer : public QObject
{
    Q_OBJECT

public:
    static IconProducer* instance();

    QIcon& disconnected() { return mDisconnected; }
    QIcon& wired_connected() { return mWired_connected; }
    QIcon& wireless(int strength);

signals:
    void iconsChanged();

private slots:
    void onIconThemeChanged();

private:
    IconProducer();
    ~IconProducer();

    QIcon buildIcon(QString pathToSvgFile);

    QIcon mWired_connected;
    QIcon mDisconnected;

    QIcon mWireless_signal_none;
    QIcon mWireless_signal_weak;
    QIcon mWireless_signal_ok;
    QIcon mWireless_signal_good;
    QIcon mWireless_signal_excellent;

    QIcon mBuilt_in_wireless_signal_none;
    QIcon mBuilt_in_wireless_signal_weak;
    QIcon mBuilt_in_wireless_signal_ok;
    QIcon mBuilt_in_wireless_signal_good;
    QIcon mBuilt_in_wireless_signal_excellent;


};



#endif // ICONPRODUCER_H

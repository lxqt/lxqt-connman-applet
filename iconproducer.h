#ifndef ICONPRODUCER_H
#define ICONPRODUCER_H
#include <QIcon>

class IconFinder : public QObject
{
    Q_OBJECT

public:
    static IconFinder* instance();

    QIcon& wired_connected() { return mWired_connected; }
    QIcon& wired_disconnected() { return mWired_disconnected; }
    QIcon& wireless(int strength);

signals:
    void iconsChanged();

private slots:
    void onIconThemeChanged();

private:
    IconFinder();
    ~IconFinder();

    QIcon buildIcon(QString pathToSvgFile);

    QIcon mWired_connected;
    QIcon mWired_disconnected;

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

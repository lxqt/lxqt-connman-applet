#include "iconfinder.h"
#include "connectionstate.h"

class IconNameFinder
{
public:
    virtual QString wirelessConnectedIconName(int strength) = 0;
    virtual QString bluetoothConnectedIconName(int strength) { return wirelessConnectedIconName(strength); }
    virtual QString ethernetConnectedIconName() = 0;
    virtual QString notConnectedIconName() = 0;
};


class IconNameFinderForOxygen : public IconNameFinder
{

public:
    virtual QString wirelessConnectedIconName(int strength)
    {
        if (strength < 13)      return "network-wireless-connected-00";
        else if (strength < 28) return "network-wireless-connected-25";
        else if (strength < 63) return "network-wireless-connected-50";
        else if (strength < 88) return "network-wireless-connected-75";
        else                    return "network-wireless-connected-100";
    }

    virtual QString ethernetConnectedIconName() { return "network-connnect";}
    virtual QString notConnectedIconName() { return "network-disconnect"; }


};

class IconNameFinderForGnome : public IconNameFinder
{
public:
    virtual QString wirelessConnectedIconName(int strength)
    {
        if (strength < 13)      return "network-wireless-signal-none";
        else if (strength < 28) return "network-wireless-signal-weak";
        else if (strength < 63) return "network-wireless-signal-ok";
        else if (strength < 88) return "network-wireless-signal-good";
        else                    return "network-wireless-signal-excellent";

    }

    virtual QString ethernetConnectedIconName() { return "network-wired"; }
    virtual QString notConnectedIconName() { return "network-offline"; }

};

IconFinder* IconFinder::instance()
{
    static IconFinder *_instance = new IconFinder();
    return _instance;
}

QIcon& IconFinder::icon()
{
	
    qDebug() << "icon(), themename:" << QIcon::themeName();
    IconNameFinder *iconNameFinder;
    if (QIcon::themeName() == "Oxygen")
    {
        qDebug() << "Oxygen";
        iconNameFinder = new IconNameFinderForOxygen();
    }
    else
    {
        qDebug() << "Gnome";
        iconNameFinder = new IconNameFinderForGnome();
    }

    QString iconName;

    ConnectionState *connectionState = ConnectionState::instance();

    if (connectionState->connectedWireless())
    {
        iconName = iconNameFinder->wirelessConnectedIconName(connectionState->connectedWireless()->signalStrength());
    }
    else if (connectionState->connectedBluetooth())
    {
        iconName = iconNameFinder->bluetoothConnectedIconName(connectionState->connectedBluetooth()->signalStrength());
    }
    else if (connectionState->connectedEthernet())
    {
        iconName = iconNameFinder->ethernetConnectedIconName();
    }
    else
    {
        iconName = iconNameFinder->notConnectedIconName();
    }

    qDebug() << "IconFinder found: "  << iconName;
    qDebug() << QIcon::hasThemeIcon(iconName);

    mIcon = QIcon::fromTheme(iconName);

    return mIcon;
}


IconFinder::IconFinder()
{
}


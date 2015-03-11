/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * lxqt-connman-applet - a gui frontend for connman
 *
 * Copyright: 2014-2015 Christian Surlykke
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include "iconfinder.h"
#include "connectionstate.h"
#include <XdgIcon>


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
    qDebug() << "xdgIcon.themeName:" << XdgIcon::themeName();
    qDebug() << "iconTheme:" <<
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


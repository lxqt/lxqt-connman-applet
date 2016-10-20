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

#include "iconproducer.h"
#include <XdgIcon>
#include <LXQt/Settings>
#include <QtSvg/QSvgRenderer>
#include <QPainter>
#include <QFile>
#include <QDebug>

IconProducer* IconProducer::instance()
{
    static IconProducer *_inst = new IconProducer();

    return _inst;
}


IconProducer::IconProducer()
{
    mBuiltInDisconnected = buildIcon(":/resources/wired-disconnected.svg");
    mBuiltInWiredConnected = buildIcon(":/resources/wired-connected.svg");
    mBuiltInWirelessNone = buildIcon(":/resources/signal-strength-0.svg");
    mBuiltInWirelessWeak = buildIcon(":/resources/signal-strength-1.svg");
    mBuiltInWirelessOk = buildIcon(":/resources/signal-strength-2.svg");
    mBuiltInWirelessGood = buildIcon(":/resources/signal-strength-3.svg");
    mBuiltInWirelessExcellent = buildIcon(":/resources/signal-strength-4.svg");

    connect(LXQt::GlobalSettings::globalSettings(), SIGNAL(iconThemeChanged()), this, SLOT(onIconThemeChanged()));
    onIconThemeChanged();
}

void IconProducer::onIconThemeChanged()
{
    // Oxygen and Breeze has these
    if (QIcon::hasThemeIcon("network-wired") &&
        QIcon::hasThemeIcon("network-connect") &&
        QIcon::hasThemeIcon("network-wireless-connected-00") &&
        QIcon::hasThemeIcon("network-wireless-connected-25") &&
        QIcon::hasThemeIcon("network-wireless-connected-50") &&
        QIcon::hasThemeIcon("network-wireless-connected-75") &&
        QIcon::hasThemeIcon("network-wireless-connected-100"))
    {
        iconThemeType = IconThemeType::oxygen;
    }
    else if (// Gnome and several other themes has these
             QIcon::hasThemeIcon("network-offline") &&
             QIcon::hasThemeIcon("network-wired") &&
             QIcon::hasThemeIcon("network-wireless-signal-none-symbolic") &&
             QIcon::hasThemeIcon("network-wireless-signal-weak-symbolic") &&
             QIcon::hasThemeIcon("network-wireless-signal-ok-symbolic") &&
             QIcon::hasThemeIcon("network-wireless-signal-good-symbolic") &&
             QIcon::hasThemeIcon("network-wireless-signal-excellent-symbolic"))
    {
        iconThemeType = IconThemeType::gnome;
    }
    else // Fallback to ugly built-in icons
    {
        iconThemeType = IconThemeType::none;
    }
    emit iconsChanged();
}


QIcon IconProducer::buildIcon(QString pathToSvgFile)
{
    QFile svgFile(pathToSvgFile);
    svgFile.open(QIODevice::ReadOnly);
    QByteArray svg = svgFile.readAll();

    QSvgRenderer render(svg);
    QPixmap pixmap(render.defaultSize());
    pixmap.fill(QColor(0,0,0,0));
    QPainter painter(&pixmap);
    render.render(&painter);
    return QIcon(pixmap);
}

QIcon IconProducer::disconnected()
{
    switch (iconThemeType) {
    case IconThemeType::oxygen: return QIcon::fromTheme("network-wired");
    case IconThemeType::gnome: return QIcon::fromTheme("network-offline");
    case IconThemeType::none: return mBuiltInDisconnected;
    }
}

QIcon IconProducer::wiredConnected()
{
    switch (iconThemeType) {
    case IconThemeType::oxygen: return QIcon::fromTheme("network-connect");
    case IconThemeType::gnome: return QIcon::fromTheme("network-wired");
    case IconThemeType::none: return mBuiltInWiredConnected;
    }
}

QIcon IconProducer::wireless(int strength)
{
    switch (iconThemeType) {
    case IconThemeType::oxygen:
        if (strength < 10)      return QIcon::fromTheme("network-wireless-connected-00");
        else if (strength < 37) return QIcon::fromTheme("network-wireless-connected-25");
        else if (strength < 63) return QIcon::fromTheme("network-wireless-connected-50");
        else if (strength < 90) return QIcon::fromTheme("network-wireless-connected-75");
        else                    return QIcon::fromTheme("network-wireless-connected-100");
    case IconThemeType::gnome:
        if (strength < 10)      return QIcon::fromTheme("network-wireless-signal-none-symbolic");
        else if (strength < 37) return QIcon::fromTheme("network-wireless-signal-weak-symbolic");
        else if (strength < 63) return QIcon::fromTheme("network-wireless-signal-ok-symbolic");
        else if (strength < 90) return QIcon::fromTheme("network-wireless-signal-good-symbolic");
        else                    return QIcon::fromTheme("network-wireless-signal-excellent-symbolic");
    case IconThemeType::none:
        if (strength < 10)      return mBuiltInWirelessNone;
        else if (strength < 37) return mBuiltInWirelessWeak;
        else if (strength < 63) return mBuiltInWirelessOk;
        else if (strength < 90) return mBuiltInWirelessGood;
        else                    return mBuiltInWirelessExcellent;

    }
}

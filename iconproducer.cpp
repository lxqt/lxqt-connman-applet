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
    mBuilt_in_wireless_signal_none      = buildIcon(":/resources/signal-strength-0.svg");
    mBuilt_in_wireless_signal_weak      = buildIcon(":/resources/signal-strength-1.svg");
    mBuilt_in_wireless_signal_ok        = buildIcon(":/resources/signal-strength-2.svg");
    mBuilt_in_wireless_signal_good      = buildIcon(":/resources/signal-strength-3.svg");
    mBuilt_in_wireless_signal_excellent = buildIcon(":/resources/signal-strength-4.svg");

    connect(LxQt::GlobalSettings::globalSettings(), SIGNAL(iconThemeChanged()), this, SLOT(onIconThemeChanged()));
    onIconThemeChanged();
}

IconProducer::~IconProducer()
{
}

void IconProducer::onIconThemeChanged()
{
    qDebug() << "themeName:" << QIcon::themeName();

    if (QIcon::themeName() == "oxygen")
    {
        mWired_connected = QIcon::fromTheme("network-connect");
        mDisconnected = QIcon::fromTheme("network-wired");

        mWireless_signal_none      = QIcon::fromTheme("network-wireless-connected-00");
        mWireless_signal_weak      = QIcon::fromTheme("network-wireless-connected-25");
        mWireless_signal_ok        = QIcon::fromTheme("network-wireless-connected-50");
        mWireless_signal_good      = QIcon::fromTheme("network-wireless-connected-75");
        mWireless_signal_excellent = QIcon::fromTheme("network-wireless-connected-100");
    }
    else
    {
        // Most themes other than oxygen seems to have these two
        mDisconnected = QIcon::fromTheme("network-offline");
        mWired_connected = QIcon::fromTheme("network-wired");

        if (QIcon::hasThemeIcon("network-wireless-signal-none-symbolic") && // Gnome (and others) uses these
            QIcon::hasThemeIcon("network-wireless-signal-weak-symbolic") &&
            QIcon::hasThemeIcon("network-wireless-signal-ok-symbolic") &&
            QIcon::hasThemeIcon("network-wireless-signal-good-symbolic") &&
            QIcon::hasThemeIcon("network-wireless-signal-excellent-symbolic"))
        {
            mWireless_signal_none =      QIcon::fromTheme("network-wireless-signal-none-symbolic");
            mWireless_signal_weak =      QIcon::fromTheme("network-wireless-signal-weak-symbolic");
            mWireless_signal_ok =        QIcon::fromTheme("network-wireless-signal-ok-symbolic");
            mWireless_signal_good =      QIcon::fromTheme("network-wireless-signal-good-symbolic");
            mWireless_signal_excellent = QIcon::fromTheme("network-wireless-signal-excellent-symbolic");
        }
        else if (QIcon::hasThemeIcon("network-wireless-signal-none") &&   // AwOken (and others) uses these
                QIcon::hasThemeIcon("network-wireless-signal-weak") &&
                QIcon::hasThemeIcon("network-wireless-signal-ok") &&
                QIcon::hasThemeIcon("network-wireless-signal-good") &&
                QIcon::hasThemeIcon("network-wireless-signal-excellent"))
        {
        mWireless_signal_none =      QIcon::fromTheme("network-wireless-signal-none");
        mWireless_signal_weak =      QIcon::fromTheme("network-wireless-signal-weak");
        mWireless_signal_ok =        QIcon::fromTheme("network-wireless-signal-ok");
        mWireless_signal_good =      QIcon::fromTheme("network-wireless-signal-good");
        mWireless_signal_excellent = QIcon::fromTheme("network-wireless-signal-excellent");
        }
        else // Fallback to ugly built-in icons
        {
            mWireless_signal_none = mBuilt_in_wireless_signal_none;
            mWireless_signal_weak = mBuilt_in_wireless_signal_weak;
            mWireless_signal_ok = mBuilt_in_wireless_signal_ok;
            mWireless_signal_good = mBuilt_in_wireless_signal_good;
            mWireless_signal_excellent = mBuilt_in_wireless_signal_excellent;
        }
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

QIcon& IconProducer::wireless(int strength)
{
    if (strength < 10)      return mWireless_signal_none;
    else if (strength < 37) return mWireless_signal_weak;
    else if (strength < 63) return mWireless_signal_ok;
    else if (strength < 90) return mWireless_signal_good;
    else                    return mWireless_signal_excellent;
}

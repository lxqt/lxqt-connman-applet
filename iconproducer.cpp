#include "iconproducer.h"
#include <XdgIcon>
#include <LXQt/Settings>
#include <QtSvg/QSvgRenderer>
#include <QPainter>
#include <QFile>
#include <QDebug>

IconFinder* IconFinder::instance()
{
    static IconFinder *_inst = new IconFinder();

    return _inst;
}


IconFinder::IconFinder()
{
    connect(LxQt::GlobalSettings::globalSettings(), SIGNAL(iconThemeChanged()), this, SLOT(onIconThemeChanged()));
    initWirelessIcons();
    onIconThemeChanged();
}

IconFinder::~IconFinder()
{
}

void IconFinder::onIconThemeChanged()
{
    qDebug() << "themeName:" << QIcon::themeName();
    if (QIcon::themeName() == "oxygen")
    {
        mWired_connected = QIcon::fromTheme("network-connect");
        mWired_disconnected = QIcon::fromTheme("network-wired");
    }
    else
    {
        mWired_connected = QIcon::fromTheme("network-transmit");
        mWired_disconnected = QIcon::fromTheme("network-wired");
    }
}


void IconFinder::initWirelessIcons()
{
    for (int i = 0; i < 6; i++)
    {
        QString filename = QString(":/resources/signal-strength-%1.svg").arg(i);
        qDebug() << "Looking for:" << filename;
        QFile svgFile(filename);
        svgFile.open(QIODevice::ReadOnly);
        QByteArray svg = svgFile.readAll();

        QSvgRenderer render(svg);
        QPixmap pixmap(render.defaultSize());
        pixmap.fill(QColor(0,0,0,0));
        QPainter painter(&pixmap);
        render.render(&painter);
        mWireless << QIcon(pixmap);
    }
}

QIcon& IconFinder::wireless(int strength)
{
    if (strength < 10) return mWireless[0];
    else if (strength < 30) return mWireless[1];
    else if (strength < 50) return mWireless[2];
    else if (strength < 70) return mWireless[3];
    else if (strength < 90) return mWireless[4];
    else                    return mWireless[5];
}

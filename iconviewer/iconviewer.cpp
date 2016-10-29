#include <LXQt/Settings>
#include "iconproducer.h"
#include "iconviewer.h"
#include "ui_iconviewer.h"

IconViewer::IconViewer(QWidget *parent): QDialog(parent), ui(new Ui::IconViewer)
{
    ui->setupUi(this);
    connect(&IconProducer::instance(), SIGNAL(iconsChanged()), this, SLOT(updateIcons()));
    updateIcons();
}

IconViewer::~IconViewer()
{
    delete ui;
}

void IconViewer::updateIcons()
{
    ui->icon_connected->setPixmap(IconProducer::instance().wiredConnected().pixmap(38,38));
    ui->icon_disconnected->setPixmap(IconProducer::instance().disconnected().pixmap(38,38));
    ui->icon_none->setPixmap(IconProducer::instance().wireless(5).pixmap(38,38));
    ui->icon_weak->setPixmap(IconProducer::instance().wireless(30).pixmap(38,38));
    ui->icon_ok->setPixmap(IconProducer::instance().wireless(50).pixmap(38,38));
    ui->icon_good->setPixmap(IconProducer::instance().wireless(80).pixmap(38,38));
    ui->icon_excellent->setPixmap(IconProducer::instance().wireless(98).pixmap(38,38));
}

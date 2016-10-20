#include "technologyframe.h"
#include "ui_technologyframe.h"

TechnologyFrame::TechnologyFrame(const QString path, const QVariantMap& properties, QWidget *parent) :
    ClickableFrame(parent),
    ui(new Ui::TechnologyFrame),
    technologyInterface("net.connman", path, QDBusConnection::systemBus(), this),
    properties(properties)

{
    ui->setupUi(this);
    connect(&technologyInterface,
            SIGNAL(PropertyChanged(QString,QDBusVariant)),
            SLOT(onPropertyChanged(QString,QDBusVariant)));
    update();
}

TechnologyFrame::~TechnologyFrame()
{
    delete ui;
}

void TechnologyFrame::click()
{
    bool newPowered = !(properties["Powered"].toBool());
    technologyInterface.SetProperty("Powered", QDBusVariant(newPowered));
}

void TechnologyFrame::onPropertyChanged(QString name, QDBusVariant value)
{
    properties[name] = value.variant();
    update();
}

void TechnologyFrame::update()
{
    ui->iconLabel->setText(properties["Powered"].toBool() ? QString(QChar(0x2713)) : "");
    ui->nameLabel->setText(properties["Name"].toString());
}


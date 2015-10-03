#include <QMouseEvent>
#include "serviceframe.h"
#include "ui_serviceframe.h"
#include "iconproducer.h"
#include "strings.h"

ServiceFrame::ServiceFrame(QString path, const QVariantMap &properties, QWidget *parent) :
    ClickableFrame(parent),
    ui(new Ui::ServiceFrame),
    properties(properties),
    serviceInterface("net.connman", path, QDBusConnection::systemBus())
{
    ui->setupUi(this);
    setMouseTracking(true);
    connect(&serviceInterface, SIGNAL(PropertyChanged(QString,QDBusVariant)),
                               SLOT(onPropertyChanged(QString,QDBusVariant)));
    setIcon();
    setName();
    setState();
}

ServiceFrame::~ServiceFrame()
{
    delete ui;
}

void ServiceFrame::click()
{
    QString state = properties["State"].toString();
    if (state == "idle" || state == "failure") {
        serviceInterface.Connect();
    }
    else if (state == "association" ||
             state == "configuration" ||
             state == "ready" ||
             state == "online") {
        serviceInterface.Disconnect();
    }
}


void ServiceFrame::onPropertyChanged(const QString &name, const QDBusVariant &value)
{
    properties[name] = value.variant();
    setIcon();
    setName();
    setState();
}

void ServiceFrame::setIcon()
{
    qDebug() << "setIcon, type() = " << type();
    QString styleSheet = "";
    QIcon icon;
    if (type() == "wifi")
    {
        int strength = properties["Strength"].toInt();
        icon = IconProducer::instance()->wireless(strength);
        if (state() == "online") {
            styleSheet =
                    "QLabel#iconLabel"
                    "{"
                    "  background-color: rgb(200,200,200);"
                    "  border-width: 1px;" "  border-style: solid;"
                    "  border-radius: 8px; "
                    "}";
        }
    }
    ui->iconLabel->setPixmap(icon.pixmap(30,30));
    ui->iconLabel->setStyleSheet(styleSheet);
}

void ServiceFrame::setName()
{
    QString name = string(properties["Name"].toString());
    qDebug() << "setName(), name =" << name;
    if (type() == "ethernet")
    {
        QString interfaceName = "FIXME";
        ui->nameLabel->setText(name + " (" + interfaceName + ")");
    }
    else
    {
        ui->nameLabel->setText(name);
    }

}

void ServiceFrame::setState()
{
    qDebug() << "setState(), state =" << state();
    if (state() == "idle" || state() == "online") {
        ui->stateLabel->hide();
    }
    else {
        ui->stateLabel->show();
        if (state() == "failure") {
            qDebug() << "Failure: " << properties["Error"];
            ui->stateLabel->setText(properties["Error"].toString());
        }
        else{
            ui->stateLabel->setText(string(state()));
        }
    }

}

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
    updateUI();
}

ServiceFrame::~ServiceFrame()
{
    delete ui;
}

void ServiceFrame::click()
{
    if (state() == "idle" || state() == "failure") {
        serviceInterface.Connect();
    }
    else if (state() == "association" ||
             state() == "configuration" ||
             state() == "ready" ||
             state() == "online") {
        serviceInterface.Disconnect();
    }
}

bool ServiceFrame::connected()
{
    return state() == "ready" || state() == "online";
}

int ServiceFrame::signalStrength()
{
    return type() == "wifi" ? properties["Strength"].toInt() : -1;
}

void ServiceFrame::onPropertyChanged(const QString &name, const QDBusVariant &value)
{
    properties[name] = value.variant();
    if (name == "State") {
        emit stateChanged();
    }
    updateUI();
}

void ServiceFrame::updateUI()
{
    if (type() == "wifi")
    {
        QString styleSheet = "";
        QIcon icon;
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
        ui->iconLabel->setPixmap(icon.pixmap(30,30));
        ui->iconLabel->setStyleSheet(styleSheet);
    }
    else {
        QString iconText = state() == "online" ? QString(QChar(0x2713)) : "";
        ui->iconLabel->setText(iconText);
    }


    QString name = string(properties["Name"].toString());
    if (type() == "ethernet")
    {
        QDBusArgument dict = properties["Ethernet"].value<QDBusArgument>();
        QVariantMap map;
        dict >> map;
        QString interfaceName = map["Interface"].toString();
        ui->nameLabel->setText(name + " (" + interfaceName + ")");
    }
    else
    {
        ui->nameLabel->setText(name);
    }

    if (state() == "idle" || state() == "online") {
        ui->stateLabel->hide();
    }
    else {
        ui->stateLabel->show();
        if (state() == "failure") {
            ui->stateLabel->setText(properties["Error"].toString());
        }
        else{
            ui->stateLabel->setText(string(state()));
        }
    }
}

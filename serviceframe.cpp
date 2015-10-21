#include <QMouseEvent>
#include "serviceframe.h"
#include "ui_serviceframe.h"
#include "iconproducer.h"
#include "strings.h"


const QString ServiceFrame::stylesheetWifiOnline =
       "QLabel#iconLabel"
       "{"
       "  background-color: rgb(230,230,230);"
       "  border-width: 2px;"
       "  border-style: solid;"
       "  border-radius: 8px; "
       "}";

const QString ServiceFrame::stylesheetWifiReady =
       "QLabel#iconLabel"
       "{"
       "  background-color: rgb(230,230,230);"
       "  border-radius: 8px; "
       "}";


const QString ServiceFrame::stylesheetOtherOnline =
       "QLabel#iconLabel"
       "{"
       "  border-width: 2px;"
       "  border-style: solid;"
       "  border-radius: 8px; "
       "}";


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
    ui->errorLabel->hide();
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

void ServiceFrame::onErrorTimeout() {
    ui->errorLabel->hide();
}

void ServiceFrame::updateUI()
{
    if (type() == "wifi")
    {
        QIcon icon;
        int strength = properties["Strength"].toInt();
        icon = IconProducer::instance()->wireless(strength);
        ui->iconLabel->setPixmap(icon.pixmap(30,30));
        if (state() == "online") {
            ui->iconLabel->setStyleSheet(stylesheetWifiOnline);
        }
        else if (state() == "ready") {
            ui->iconLabel->setStyleSheet(stylesheetWifiReady);
        }
        else {
            ui->iconLabel->setStyleSheet("");
        }
    }
    else {
        if (state() == "online" || state() == "ready") {
            ui->iconLabel->setText(QString(QChar(0x2713))); // Checkmark
        }
        else {
            ui->iconLabel->clear();
        }

        if (state() == "online") {
            ui->iconLabel->setStyleSheet(stylesheetOtherOnline);
        }
        else {
            ui->iconLabel->setStyleSheet("");
        }
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

    if (state() == "idle" || state() == "ready" || state() == "online") {
        ui->stateLabel->hide();
    }
    else if (state() == "failure") {
            ui->errorLabel->setText(string(properties["Error"].toString()));
            ui->errorLabel->show();
            QTimer::singleShot(4000, this, SLOT(onErrorTimeout()));
        }
    else {
        ui->stateLabel->setText(string(state()));
        ui->stateLabel->show();
    }

    if (state() != "idle" && state() != "failure") {
        ui->errorLabel->hide();
    }
}

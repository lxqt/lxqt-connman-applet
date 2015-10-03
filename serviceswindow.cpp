#include <QMouseEvent>
#include "serviceswindow.h"
#include "ui_serviceswindow.h"
#include "serviceframe.h"
#include "technologyframe.h"

ServicesWindow::ServicesWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServicesWindow),
    managerInterface("net.connman", "/", QDBusConnection::systemBus()),
    technologies(),
    services(),
    servicePaths(),
    selectedFrame(0),
    pressedFrame(0)
{
    ui->setupUi(this);
    connect(&managerInterface,
            SIGNAL(TechnologyAdded(QDBusObjectPath,QVariantMap)),
            SLOT(onTechnologyAdded(QDBusObjectPath,QVariantMap)));

    connect(&managerInterface,
            SIGNAL(TechnologyRemoved(QDBusObjectPath)),
            SLOT(onTechnologyRemoved(QDBusObjectPath)));

    connect(&managerInterface,
            SIGNAL(ServicesChanged(ObjectPropertiesList,QList<QDBusObjectPath>)),
            SLOT(onServicesChanged(ObjectPropertiesList,QList<QDBusObjectPath>)));

    for (ObjectProperties pathPropsPair : ObjectPropertiesList(managerInterface.GetTechnologies())) {
        onTechnologyAdded(pathPropsPair.first, pathPropsPair.second);
    }

    onServicesChanged(ObjectPropertiesList(managerInterface.GetServices()), QList<QDBusObjectPath>());
    setMouseTracking(true);
}

ServicesWindow::~ServicesWindow()
{
    delete ui;
}

void ServicesWindow::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << "Release: " << event->key() << ", text:" << event->text();
    qDebug() << "'k'" << (int) 'k';
    qDebug() << "arrow-up" << Qt::UpArrow;
    int m,n;
    getSelected(m, n);
    if (event->key() == Qt::Key_Return ||
        event->key() == Qt::Key_Enter ||
        event->text() == " ") {
        if (selectedFrame) {
            selectedFrame->click();
        }
    }
    else {
        if (event->key() == Qt::Key_Up || event->key() == Qt::Key_K) {
            up(m,n);
            select(m,n);
        }
        else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_J) {
            down(m,n);
            select(m,n);
        }
    }
}

void ServicesWindow::onTechnologyAdded(const QDBusObjectPath &path, const QVariantMap &properties)
{
    if (! technologies.contains(path.path())) {
        TechnologyFrame *technologyFrame = new TechnologyFrame(path.path(), properties);
        connect(technologyFrame, SIGNAL(entered()), SLOT(onFrameEntered()));
        connect(technologyFrame, SIGNAL(left()), SLOT(onFrameLeft()));
        connect(technologyFrame, SIGNAL(pressed()), SLOT(onFramePressed()));
        connect(technologyFrame, SIGNAL(released()), SLOT(onFrameReleased()));

        technologies[path.path()] = technologyFrame;
        ui->technologiesLayout->addWidget(technologyFrame);
    }

    ui->noTechnologiesLabel->setVisible(technologies.isEmpty());
}

void ServicesWindow::onTechnologyRemoved(const QDBusObjectPath &path)
{
    if (technologies.contains(path.path())) {
        technologies.take(path.path())->deleteLater();
    }

    ui->noTechnologiesLabel->setVisible(technologies.isEmpty());
}

void ServicesWindow::onServicesChanged(ObjectPropertiesList changed, QList<QDBusObjectPath> removed)
{
    servicePaths.clear();
    while (! ui->servicesLayout->isEmpty()) {
        ui->servicesLayout->takeAt(0);
    }

    for (QDBusObjectPath path : removed) {
        if (services.contains(path.path())) {
            services.take(path.path())->deleteLater();
        }
    }

    for (ObjectProperties pathPropPair : changed) {
        QString path = pathPropPair.first.path();
        QVariantMap props = pathPropPair.second;
        servicePaths.append(path);
        if (! services.contains(path)) {
            services[path] = new ServiceFrame(path, props, this);
            connect(services[path], SIGNAL(entered()), SLOT(onFrameEntered()));
            connect(services[path], SIGNAL(left()), SLOT(onFrameLeft()));
            connect(services[path], SIGNAL(pressed()), SLOT(onFramePressed()));
            connect(services[path], SIGNAL(released()), SLOT(onFrameReleased()));
        }
        ui->servicesLayout->addWidget(services[path]);
    }

    ui->noServicesLabel->setVisible(servicePaths.isEmpty());
}

void ServicesWindow::onFrameEntered()
{
   select(dynamic_cast<ClickableFrame*>(sender()));
}

void ServicesWindow::onFrameLeft()
{
    deselect();
}

void ServicesWindow::onFramePressed()
{
    pressedFrame = dynamic_cast<ClickableFrame*>(sender());
    pressedFrame->setStyleSheet("QFrame{background-color: rgb(120, 120, 120);}");
}

void ServicesWindow::onFrameReleased()
{
    if (pressedFrame) {
        pressedFrame->setStyleSheet("");
    }
    ClickableFrame* frame = dynamic_cast<ClickableFrame*>(sender());
    if (frame == pressedFrame) {
        pressedFrame->click();
    }
}

void ServicesWindow::getSelected(int &m, int &n)
{
    m = 0;
    n = -1;
    if (selectedFrame) {
        for (int i = 0; i < ui->technologiesLayout->count(); i++) {
            if (selectedFrame == ui->technologiesLayout->itemAt(i)->widget()) {
                n = i;
                return;
            }
        }
        m = 1;
        for (int i = 0; i < ui->servicesLayout->count(); i++) {
            if (selectedFrame == ui->servicesLayout->itemAt(i)->widget()) {
                n = i;
                return;
            }
        }
    }
}

void ServicesWindow::up(int &m, int &n)
{
    if (n < 0) {
        m = 1;
        n = ui->servicesLayout->count() - 1;
    }
    else if (n == 0) {
        if (m == 0) {
            n = -1;
        }
        else {
            m = 0;
            n = ui->technologiesLayout->count() - 1;
        }
    }
    else {
        n--;
    }

}

void ServicesWindow::down(int &m, int &n)
{
    if (n < 0) {
        m = 0;
        n = 0;
    }
    else if (m == 0 && n >= ui->technologiesLayout->count() - 1) {
        m = 1;
        n = 0;
    }
    else if (m == 1 && n >= ui->servicesLayout->count() - 1) {
        n = -1;
    }
    else {
        n++;
    }
}

void ServicesWindow::deselect()
{
    if (selectedFrame) {
        selectedFrame->setStyleSheet("");
        selectedFrame = 0;
    }
}

void ServicesWindow::select(ClickableFrame *frame)
{
    deselect();
    selectedFrame = frame;
    if (frame) {
        frame->setStyleSheet("QFrame{background-color: rgb(220, 220, 220);}");
    }
}

void ServicesWindow::select(int m, int n) {
    ClickableFrame *frame = 0;
    if (n != -1) {
        if (m == 0) {
            frame = dynamic_cast<ClickableFrame*>(ui->technologiesLayout->itemAt(n)->widget());
        }
        else {
            frame = dynamic_cast<ClickableFrame*>(ui->servicesLayout->itemAt(n)->widget());
        }
    }

    select(frame);
}

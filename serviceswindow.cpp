#include <QMouseEvent>
#include <QMessageBox>
#include <QMenu>
#include "serviceswindow.h"
#include "ui_serviceswindow.h"

ServicesWindow::ServicesWindow(QAbstractItemModel* technologiesListModel,
                               QAbstractItemModel* servicesListModel,
                               QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServicesWindow)
{
    ui->setupUi(this);
    ui->technologiesListView->setModel(technologiesListModel);
    ui->servicesListView->setModel(servicesListModel);
}

ServicesWindow::~ServicesWindow()
{
    delete ui;
}

/*void ServicesWindow::about()
{
    QMessageBox::about(0,
                       tr("About"),
                       tr( "<p>"
                           "  <b>LXQt Connman Client</b>"
                           "</p>"
                           "<p>"
                           "Copyright 2014, 2015"
                           "</p>"
                           "<p>"
                           "Christian Surlykke"
                           "</p>"
                           ));
}*/

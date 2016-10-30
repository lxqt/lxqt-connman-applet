#include <QMouseEvent>
#include <QMessageBox>
#include <QMenu>
#include "serviceswindow.h"
#include "ui_serviceswindow.h"

ServicesWindow::ServicesWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServicesWindow)
{
    ui->setupUi(this);
    connect(ui->treeView, SIGNAL(activated(const QModelIndex&)), SIGNAL(activated(const QModelIndex&)));
}

ServicesWindow::~ServicesWindow()
{
    delete ui;
}

void ServicesWindow::setModel(QAbstractItemModel* model)
{
    ui->treeView->setModel(model);
    ui->treeView->expandAll();
}

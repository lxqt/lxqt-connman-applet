#ifndef SERVICESWINDOW_H
#define SERVICESWINDOW_H

#include <QDialog>
#include <QAbstractItemModel>

namespace Ui {
class ServicesWindow;
}

class ServicesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ServicesWindow(QWidget *parent = 0);
    ~ServicesWindow();
    void setModel(QAbstractItemModel* model);

signals:
    void activated(const QModelIndex& index);

private:
    Ui::ServicesWindow *ui;

};

#endif // SERVICESWINDOW_H

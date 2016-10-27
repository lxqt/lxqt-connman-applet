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
    explicit ServicesWindow(QAbstractItemModel* technologiesListModel,
                            QAbstractItemModel* servicesListModel,
                            QWidget *parent = 0);
    ~ServicesWindow();

private:
    Ui::ServicesWindow *ui;

};

#endif // SERVICESWINDOW_H

#ifndef SERVICESWINDOW_H
#define SERVICESWINDOW_H

#include <QDialog>
#include <QMap>
#include <QHash>
#include <QMouseEvent>
#include "dbus_types.h"
#include "net.connman.Manager.h"
#include "clickableframe.h"

namespace Ui {
class ServicesWindow;
}

class TechnologyFrame;
class ServiceFrame;
class ServicesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ServicesWindow(QWidget *parent = 0);
    ~ServicesWindow();

public slots:
    void toggleShow();

protected:
    virtual void keyReleaseEvent(QKeyEvent *event);

private slots:
    void onTechnologyAdded(const QDBusObjectPath &path, const QVariantMap &properties);
    void onTechnologyRemoved(const QDBusObjectPath &path);
    void onServicesChanged(ObjectPropertiesList changed, QList<QDBusObjectPath> removed);
    void onFrameEntered();
    void onFrameLeft();
    void onFramePressed();
    void onFrameReleased();

private:
    void getSelected(int &m, int &n);
    void up(int &m, int &n);
    void down(int &m, int &n);
    void deselect();
    void select(ClickableFrame *frame);
    void select(int m, int n);

    NetConnmanManagerInterface managerInterface;

    Ui::ServicesWindow *ui;

    QMap<QString, TechnologyFrame*> technologies;

    QHash<QString, ServiceFrame*> services;
    QList<QString> servicePaths;

    ClickableFrame *selectedFrame;
    ClickableFrame *pressedFrame;
};

#endif // SERVICESWINDOW_H

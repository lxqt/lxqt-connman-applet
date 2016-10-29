#ifndef ICONVIEWER_H
#define ICONVIEWER_H

#include <QDialog>

namespace Ui {
class IconViewer;
}

class IconViewer : public QDialog
{
    Q_OBJECT

public:
    explicit IconViewer(QWidget *parent = 0);
    ~IconViewer();

public slots:
    void updateIcons();

private:
    Ui::IconViewer *ui;
};

#endif // ICONVIEWER_H

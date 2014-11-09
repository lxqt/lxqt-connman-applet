#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QString>

#include "ui_dialog.h"

class QLineEdit;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog, Ui::Dialog
{
    Q_OBJECT

public:
    explicit Dialog(QString service, QVariantMap request, QWidget *parent = 0);
    ~Dialog();

    QVariantMap collectedInput();

private:
    QString uiString(QString key);
    QMap<QString, QLineEdit*> inputFields;
};

#endif // DIALOG_H

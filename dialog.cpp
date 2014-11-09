#include "dialog.h"
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

Dialog::Dialog(QString service, QVariantMap request, QWidget *parent) :
    QDialog(parent), Ui::Dialog()
{
    setupUi(this);
    headingLabel->setText(headingLabel->text().arg(service));

    foreach (QString key, request.keys())
    {
        QLabel *label = new QLabel(uiString(key), this);
        QLineEdit *lineEdit = new QLineEdit(this);
        inputFields[key] = lineEdit;
        inputFieldsLayout->addRow(label, lineEdit);
    }
}

Dialog::~Dialog()
{
}

QVariantMap Dialog::collectedInput()
{
    QVariantMap collected;

    foreach (QString key, inputFields.keys())
    {
        collected[key] = inputFields[key]->text(); // FIXME Handle bytearrays
    }

    return collected;
}

QString Dialog::uiString(QString key)
{
    return key; // FIXME
}

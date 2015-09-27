/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * lxqt-connman-applet - a gui frontend for connman
 *
 * Copyright: 2014-2015 Christian Surlykke
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

#include "strings.h"

#include "dialog.h"

Dialog::Dialog(QString service, QVariantMap request, QWidget *parent) :
    QDialog(parent), Ui::Dialog()
{
    setupUi(this);
    headingLabel->setText(headingLabel->text().arg(service));

    foreach (QString key, request.keys())
    {
        QLabel *label = new QLabel(string(key), this);
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


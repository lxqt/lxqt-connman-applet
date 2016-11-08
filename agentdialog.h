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

#ifndef AGENTDIALOG_H
#define AGENTDIALOG_H

#include <QDialog>
#include <QString>

#include "ui_agentdialog.h"

class QLineEdit;

namespace Ui {
class Dialog;
}

class AgentDialog : public QDialog, Ui::Dialog
{
    Q_OBJECT

public:
    explicit AgentDialog(QString service, QVariantMap request, QWidget *parent = 0);
    ~AgentDialog();

    QVariantMap collectedInput();

private:
    QMap<QString, QLineEdit*> inputFields;
};

#endif // AGENTDIALOG_H

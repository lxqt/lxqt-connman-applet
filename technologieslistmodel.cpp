#include <QDebug>
#include <QDBusReply>
#include <QFont>
#include "dbus_types.h"
#include "iconproducer.h"
#include "technologieslistmodel.h"

TechnologiesListModel::TechnologiesListModel(QObject *parent) :
    QAbstractListModel(parent),
    technologies()
{
}

TechnologiesListModel::~TechnologiesListModel()
{
}

int TechnologiesListModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : technologies.size();
}


QVariant TechnologiesListModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid() && index.row() < technologies.size()) {
        ConnmanObject& technology = *technologies[index.row()];
        switch(role) {
        case Qt::DisplayRole:
            return technology["Name"];
        case Qt::DecorationRole:
            if (technology["Type"] == "ethernet") {
                return IconProducer::instance().wiredConnected();
            }
            else if (technology["Type"] == "wifi") {
                return IconProducer::instance().wireless(80);
            }
            else {
                return IconProducer::instance().blanc();
            }
        case Qt::FontRole:
            if (technology["Powered"].toBool()) {
                return QVariant();
            }
            else {
                QFont font;
                font.setItalic(true);
                return font;
            }
        case Qt::ForegroundRole:
             if (technology["Powered"].toBool()) {
                return QVariant();
            }
            else {
                return QColor("lightgrey");
            }
        default:
            return QVariant();
        }
    }
    else  {
        return QVariant();
    }
}

void TechnologiesListModel::onTechnologyAdded(const QDBusObjectPath& path, const QVariantMap& properties)
{
    int i = 0;
    while (i < technologies.size() && path.path() < technologies[i]->path()) {
        i++;
    }

    if (i < technologies.size() && path.path() == technologies[i]->path()) {
        return;
    }

    ConnmanObject* technology = new ConnmanObject(path.path(), "net.connman.Technology");
    connect(technology, SIGNAL(PropertyChanged(const QString&, const QDBusVariant&)),
                        SLOT(onTechnologyPropertyChanged(const QString&, const QDBusVariant&)));
    for (const QString& key : properties.keys()) {
        (*technology)[key] = properties[key];
    }
    beginInsertRows(QModelIndex(), i, i);
    technologies.insert(i, technology);
    endInsertRows();
}

void TechnologiesListModel::onTechnologyRemoved(const QDBusObjectPath& path)
{
    for (int i = 0; i < technologies.size(); i++) {
        if (path.path() == technologies[i]->path()) {
            emit layoutAboutToBeChanged();
            technologies.takeAt(i)->deleteLater();
            emit layoutChanged();
            return;
        }
    }
}

void TechnologiesListModel::onTechnologyPropertyChanged(const QString& name, const QDBusVariant& newValue)
{
    ConnmanObject& technology = *dynamic_cast<ConnmanObject*>(sender());
    technology[name] = newValue.variant();
    emit layoutChanged();
}

void TechnologiesListModel::onTechnologyActivated(const QModelIndex& index)
{
    if (index.isValid() && index.row() < technologies.size()) {
        ConnmanObject& technology = *technologies[index.row()];
        bool newPowered = !(technology["Powered"].toBool());
        technology.asyncCall("SetProperty", QVariant("Powered"), QVariant::fromValue(QDBusVariant(newPowered)));
    }
}

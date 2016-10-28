#include <QDebug>
#include <QDBusReply>
#include "dbus_types.h"
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
    connect(technology, SIGNAL(PropertyChanged(const QString&, const QVariant&)),
                        SLOT(onTechnologyPropertyChanged(const QString&, const QVariant&)));
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

void TechnologiesListModel::onTechnologyPropertyChanged(const QString& name, const QVariant& newValue)
{
    qDebug() << "technology updated: " << name << " -> " << newValue;
    ConnmanObject& technology = *dynamic_cast<ConnmanObject*>(sender());
    technology[name] = newValue;
}

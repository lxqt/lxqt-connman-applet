#include <QDebug>
#include <QDBusReply>
#include "dbus_types.h"
#include "net.connman.Technology.h"
#include "technologieslistmodel.h"

struct TechnologiesListModelPrivate
{
    QVector<Technology*> technologies;
};

TechnologiesListModel::TechnologiesListModel(QObject *parent) :
    QAbstractListModel(parent),
    modelData(new TechnologiesListModelPrivate())
{
}

TechnologiesListModel::~TechnologiesListModel()
{
    delete(modelData);
}

int TechnologiesListModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : modelData->technologies.size();
}


QVariant TechnologiesListModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid() && index.row() < modelData->technologies.size() && role == Qt::DisplayRole) {
        return modelData->technologies[index.row()]->properties["Name"];
    }
    else  {
        return QVariant();
    }
}

void TechnologiesListModel::onTechnologyAdded(const QDBusObjectPath& path, const QVariantMap& properties)
{
    int i = 0;
    while (i < modelData->technologies.size() && path.path() < modelData->technologies[i]->path()) {
        i++;
    }

    if (i < modelData->technologies.size() && path.path() == modelData->technologies[i]->path()) {
        return;
    }

    Technology* technology = new Technology(path.path(), properties);
    connect(technology, SIGNAL(PropertyChanged(const QString&, const QDBusVariant&)),
                        SLOT(onTechnologyUpdated(const QString&, const QDBusVariant&)));

    beginInsertRows(QModelIndex(), i, i);
    modelData->technologies.insert(i, technology);
    endInsertRows();
}

void TechnologiesListModel::onTechnologyRemoved(const QDBusObjectPath& path)
{
    for (int i = 0; i < modelData->technologies.size(); i++) {
        if (path.path() == modelData->technologies[i]->path()) {
           beginRemoveColumns(QModelIndex(), i, i);
           modelData->technologies.takeAt(i)->deleteLater();
           endRemoveRows();
           return;
        }
    }
}

void TechnologiesListModel::onTechnologyUpdated(const QString& name, const QDBusVariant& newValue)
{
    qDebug() << "technology updated: " << name << " -> " << newValue.variant();
    Technology* technology = dynamic_cast<Technology*>(sender());
    technology->properties[name] = newValue.variant();
}

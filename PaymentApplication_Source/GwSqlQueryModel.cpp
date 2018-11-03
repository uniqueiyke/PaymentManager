#include "GwSqlQueryModel.h"
#include <QDebug>
#include <QDataStream>
#include <QSqlQuery>

int GwSqlQueryModel::counter_ = 0;

GwSqlQueryModel::GwSqlQueryModel(QObject *parent, QSqlDatabase db, EDITABILITY editability)
    : QSqlTableModel(parent, db)
{
    setEditability(editability);
    connect(this, &GwSqlQueryModel::editabilityChanged, this, &GwSqlQueryModel::resetFlags);
}

void GwSqlQueryModel::setEditability(GwSqlQueryModel::EDITABILITY editability)
{
    editability_ = editability;
    emit editabilityChanged(editability, QModelIndex());

}

GwSqlQueryModel::EDITABILITY GwSqlQueryModel::editability() const
{
    return editability_;
}

void GwSqlQueryModel::setEditableRows(const QList<int> &rows)
{
    editableRows_ = rows;
}

QList<int> GwSqlQueryModel::editableRows() const
{
    return editableRows_;
}

void GwSqlQueryModel::setEditableColumns(const QList<int> &columns)
{
    editableColumns_ = columns;
}

QList<int> GwSqlQueryModel::editableColumns() const
{
    return editableColumns_;
}

Qt::ItemFlags GwSqlQueryModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    switch (editability_) {
    case EDITABILITY::FULLY_EDITABLE:
        flags |= Qt::ItemIsEditable;
        return flags;
    case EDITABILITY::PARTLY_EDITABLE_ROWS:
        if(editableRows_.contains(index.row())){
            flags |= Qt::ItemIsEditable;
        }
        return flags;
    case EDITABILITY::PARTLY_EDITABLE_COLUMNS:
        if(editableColumns_.contains(index.column())){
            flags |= Qt::ItemIsEditable;
        }
        return flags;
    default:
        return flags;
    }
}

void GwSqlQueryModel::setQuery(const QSqlQuery &query)
{
    QSqlTableModel::setQuery(query);
}

void GwSqlQueryModel::setQuery(const QString &query, const QSqlDatabase &db)
{
    setQuery(QSqlQuery(query, db));
}

void GwSqlQueryModel::resetFlags(GwSqlQueryModel::EDITABILITY editability, const QModelIndex &index)
{
    Q_UNUSED(editability);
    flags(index);
}

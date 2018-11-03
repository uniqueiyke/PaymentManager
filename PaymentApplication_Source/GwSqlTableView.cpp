#include "GwSqlTableView.h"
#include "GwSqlQueryModel.h"
#include <QInputDialog>
#include <QTimer>
#include <QDebug>

GwSqlTableView::GwSqlTableView(QWidget *parent)
    : QTableView(parent),  tableModel_(new GwSqlQueryModel)
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [&](){
        emit stateChanged();
    });
    timer->start(1000);

    connect(this, &GwSqlTableView::stateChanged, [&](){
        if(isDirty()){
            setWindowModified(true);
        }else {
            setWindowModified(false);
        }
    });
}

GwDatabaseCreator &GwSqlTableView::dbCreator()
{
    return dbCreator_;
}

void GwSqlTableView::setDbCreator(const GwDatabaseCreator &dbCreator)
{
    dbCreator_ = dbCreator;
}

GwSqlQueryModel *GwSqlTableView::tableModel() const
{
    return tableModel_;
}

void GwSqlTableView::setTableModel(GwSqlQueryModel *tableModel)
{
    tableModel_ = tableModel;
    setModel(tableModel_);
}

void GwSqlTableView::reSetTableModel()
{
    tableModel_ = new GwSqlQueryModel(this, dbCreator_.db(), GwSqlQueryModel::EDITABILITY::NOT_EDITABLE);
     tableModel_->setEditStrategy(QSqlTableModel::OnManualSubmit);
    setModel(tableModel_);
}

void GwSqlTableView::refresh(const QString &tableName)
{
    tableModel_->setTable(tableName);
    tableModel_->select();
    setHeaders();
}

bool GwSqlTableView::isDirty()
{
    auto value = tableModel_->isDirty();
    return value;
}

void GwSqlTableView::onEditabilityChange()
{

    QInputDialog combo(this);
    combo.setComboBoxItems(QStringList{"No Edit", "Full Edit", "Edit Some Column", "Edit Some Row"});

    int i = combo.exec();

    if(i == 1){
        QString s = combo.textValue();

        if(s == "Full Edit"){
            tableModel_->setEditability(GwSqlQueryModel::EDITABILITY::FULLY_EDITABLE);
            emit readyToEdit();
        }else if(s == "Edit Some Column"){
            tableModel_->setEditability(GwSqlQueryModel::EDITABILITY::PARTLY_EDITABLE_COLUMNS);
            emit readyToEdit();
        }else if(s == "Edit Some Row"){
            tableModel_->setEditability(GwSqlQueryModel::EDITABILITY::PARTLY_EDITABLE_ROWS);
            emit readyToEdit();
        }else{
            tableModel_->setEditability(GwSqlQueryModel::EDITABILITY::NOT_EDITABLE);
        }
    }


}

bool GwSqlTableView::onSaveData(bool value)
{
    if(value)
        return tableModel_->submitAll();

    tableModel_->revertAll();
    return false;
}

bool GwSqlTableView::viewState() const
{
    return viewState_;
}

void GwSqlTableView::setViewState(bool viewState)
{
    viewState_ = viewState;
}

void GwSqlTableView::setHeaders()
{
    QStringList labels;
    labels << "Reg. Number" << "Name" << "Class" << "Amount" << "Paid For" << "Date Of Payment";
    int counter = 0;
    for(auto label : labels){
        tableModel_->setHeaderData(++counter, Qt::Horizontal, QString(tr("%1")).arg(label));
    }
    resizeColumnsToContents();
    hideColumn(0);
}

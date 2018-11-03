#ifndef GWSQLTABLEVIEW_H
#define GWSQLTABLEVIEW_H

#include <QTableView>
#include "GwDatabaseCreator.h"

class GwSqlQueryModel;


class GwSqlTableView : public QTableView
{
    Q_OBJECT
public:
    explicit GwSqlTableView(QWidget *parent = nullptr);
    GwDatabaseCreator & dbCreator();
    void setDbCreator(const GwDatabaseCreator &dbCreator);

    GwSqlQueryModel *tableModel() const;
    void setTableModel(GwSqlQueryModel *tableModel);
    void reSetTableModel();
    void refresh(const QString &tableName);
    bool isDirty();

    bool viewState() const;
    void setViewState(bool viewState);

signals:
    void readyToEdit();
    void stateChanged();

public slots:
    void onEditabilityChange();
    bool onSaveData(bool value = true);

private:
    GwDatabaseCreator dbCreator_;
    GwSqlQueryModel *tableModel_;
    bool viewState_;

    void setHeaders();
};

#endif // GWSQLTABLEVIEW_H

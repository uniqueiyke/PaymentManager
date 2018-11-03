#ifndef GWSQLQUERYMODEL_H
#define GWSQLQUERYMODEL_H

#include <QSqlTableModel>

class GwSqlQueryModel : public QSqlTableModel
{
    Q_OBJECT

public:

    enum class EDITABILITY {NOT_EDITABLE = 0, PARTLY_EDITABLE_ROWS, PARTLY_EDITABLE_COLUMNS, FULLY_EDITABLE};
    Q_ENUM(EDITABILITY)

    explicit GwSqlQueryModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase(), EDITABILITY editability = EDITABILITY::NOT_EDITABLE);

    void setEditability(EDITABILITY editability);
    EDITABILITY editability() const;

    void setEditableRows(const QList<int> &rows);
    QList<int> editableRows() const;

    void setEditableColumns(const QList<int> &columns);
    QList<int> editableColumns() const;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

signals:
    void editabilityChanged(EDITABILITY editability, const QModelIndex &index);
    void modelReady();


public slots:

    void setQuery(const QSqlQuery &query);
    void setQuery(const QString &query, const QSqlDatabase &db = QSqlDatabase());

private slots:
    void resetFlags(EDITABILITY editability, const QModelIndex &index);

private:
    EDITABILITY editability_;
    QList<int> editableRows_;
    QList<int> editableColumns_;
    static int counter_;
};

#endif // GWSQLQUERYMODEL_H

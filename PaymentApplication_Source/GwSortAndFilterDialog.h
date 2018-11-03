#ifndef GWSORTANDFILTERDIALOG_H
#define GWSORTANDFILTERDIALOG_H

#include <QDialog>
#include <GwSqlClauses.h>

class QComboBox;
class QSqlQueryModel;

namespace Ui {
class GwSortAndFilterDialog;
}

class GwSortAndFilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GwSortAndFilterDialog(const QSqlQueryModel *model, QWidget *parent = nullptr);
    ~GwSortAndFilterDialog();
    void setFieldBoxItems();

    //return values
    QString sortOrderComboBoxValue() const;
    QString sortFieldComboBoxValue() const;
    QString secondSortOrderComboBoxValue() const;
    QString secondSortFieldComboBoxValue() const;

    QList<GwWhereClause> tableSortOrder();
    QList<GwWhereClause> tableFilters();


    QString filterComboBoxValue() const;
    QString moreFilterComboBoxValue() const;
    QString moreFilterComboBoxValue1() const;
    QString filterField() const;
    QString moreFilterField() const;
    QString moreFilterField1() const;

signals:
    void comboBoxTextChange(QComboBox * const comboBox, const QString &text);

private slots:
    void onToggledAdvancePushButton(bool state);
    void onComboBoxTextChange(QComboBox * const comboBox, const QString &value);

private:
    const QSqlQueryModel *model_;
    Ui::GwSortAndFilterDialog *ui;

    void widgtSettings();
};

#endif // GWSORTANDFILTERDIALOG_H

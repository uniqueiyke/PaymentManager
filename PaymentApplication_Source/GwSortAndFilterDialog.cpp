#include "GwSortAndFilterDialog.h"
#include "ui_GwSortAndFilterDialog.h"
#include <QCheckBox>
#include <QListWidgetItem>
#include <QDebug>
#include <QtSql>

GwSortAndFilterDialog::GwSortAndFilterDialog(const QSqlQueryModel *model, QWidget *parent) :
    QDialog(parent), model_(model),
    ui(new Ui::GwSortAndFilterDialog)
{
    ui->setupUi(this);
    widgtSettings();
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

GwSortAndFilterDialog::~GwSortAndFilterDialog()
{
    delete ui;
}

void GwSortAndFilterDialog::setFieldBoxItems()
{
    QSqlRecord record = model_->record();
    int fieldCount = record.count();
    QStringList listOfFields;
    for(int c{}; c < fieldCount; ++c){
        listOfFields << record.fieldName(c);
    }

    ui->mainSortFieldComboBox->addItem("Select Sort Field");
    ui->mainSortFieldComboBox->addItems(listOfFields);
    ui->advanceSortFieldComboBox->addItem("Select Sort Field");
    ui->advanceSortFieldComboBox->addItems(listOfFields);

    ui->filterComboBox->addItem("Select Filter Field");
    ui->filterComboBox->addItems(listOfFields);
    ui->moreFilterComboBox->addItem("Select Filter Field");
    ui->moreFilterComboBox->addItems(listOfFields);
    ui->moreFilterComboBox_1->addItem("Select Filter Field");
    ui->moreFilterComboBox_1->addItems(listOfFields);


}

void GwSortAndFilterDialog::onComboBoxTextChange(QComboBox * const comboBox, const QString &value)
{
    QStringList list;
    auto rowCount = model_->rowCount();

    if(value == "Select Filter Field"){
        comboBox->clear();
        comboBox->addItem("Select Filter Value");
        return;
    }

    for(int r{}; r < rowCount; ++r){
        QString v = model_->record(r).value(value).toString();
        if(!list.contains(v))
        list << v;
    }

    comboBox->clear();
    comboBox->addItem("Select Filter Value");
    comboBox->addItems(list);
}

QString GwSortAndFilterDialog::sortFieldComboBoxValue() const
{
    if(ui->mainSortFieldComboBox->currentText() == "Select Sort Field")
        return "";
    return ui->mainSortFieldComboBox->currentText();
}

QString GwSortAndFilterDialog::secondSortFieldComboBoxValue() const
{
    if(ui->advanceSortFieldComboBox->currentText() == "Select Sort Field")
        return "";
    return ui->advanceSortFieldComboBox->currentText();
}

QString GwSortAndFilterDialog::filterComboBoxValue() const
{
    if(ui->filterValueComboBox->currentText() == "Select Filter Value")
        return "";
    return ui->filterValueComboBox->currentText();
}

QString GwSortAndFilterDialog::moreFilterComboBoxValue() const
{
    if(ui->moreFilterValueComboBox->currentText() == "Select Filter Value")
        return "";
    return ui->moreFilterValueComboBox->currentText();
}

QString GwSortAndFilterDialog::moreFilterComboBoxValue1() const
{
    if(ui->moreFilterValueComboBox_1->currentText() == "Select Filter Value")
        return "";
    return ui->moreFilterValueComboBox_1->currentText();
}

QString GwSortAndFilterDialog::sortOrderComboBoxValue() const
{
    if(ui->mainSortOrderComboBox->currentText() == "Ascending")
        return "ASC";
    return "DESC";
}

QString GwSortAndFilterDialog::secondSortOrderComboBoxValue() const
{
    if(ui->advanceSortOrderComboBox->currentText() == "Ascending")
        return "ASC";
    return "DESC";
}

QList<GwWhereClause> GwSortAndFilterDialog::tableSortOrder()
{
    QList<GwWhereClause> list;
    if(!sortFieldComboBoxValue().isEmpty()){
        list << GwWhereClause (sortFieldComboBoxValue(), sortOrderComboBoxValue());
    }
    if(!secondSortFieldComboBoxValue().isEmpty()){
        list << GwWhereClause (secondSortFieldComboBoxValue(), secondSortOrderComboBoxValue());
    }
    return  list;
}

QList<GwWhereClause> GwSortAndFilterDialog::tableFilters()
{
    QList<GwWhereClause> list;
    if(!filterField().isEmpty() && !filterComboBoxValue().isEmpty()){
        list << GwWhereClause(filterField(), filterComboBoxValue());
    }
    if(!moreFilterField().isEmpty() && !moreFilterComboBoxValue().isEmpty()){
        list << GwWhereClause(moreFilterField(), moreFilterComboBoxValue());
    }
    if(!moreFilterField1().isEmpty() && !moreFilterComboBoxValue1().isEmpty()){
        list << GwWhereClause(moreFilterField1(), moreFilterComboBoxValue1());
    }
    return  list;
}

QString GwSortAndFilterDialog::filterField() const
{
    if(ui->filterComboBox->currentText() == "Select Filter Field")
        return "";
    return ui->filterComboBox->currentText();
}

QString GwSortAndFilterDialog::moreFilterField() const
{
    if(ui->moreFilterComboBox->currentText() == "Select Filter Field")
        return "";
    return ui->moreFilterComboBox->currentText();
}

QString GwSortAndFilterDialog::moreFilterField1() const
{
    if(ui->moreFilterComboBox_1->currentText() == "Select Filter Field")
        return "";
    return ui->moreFilterComboBox_1->currentText();
}

void GwSortAndFilterDialog::widgtSettings()
{

    ui->advanceGroupBox->hide();
    ui->filterGroupBox_2->hide();
    ui->filterGroupBox_3->hide();

    connect(ui->advancePushButton, &QPushButton::toggled, this, &GwSortAndFilterDialog::onToggledAdvancePushButton);
    connect(ui->OkPushButton, &QPushButton::clicked, this, &GwSortAndFilterDialog::accept);
    connect(ui->cancelPushButton, &QPushButton::clicked, this, &GwSortAndFilterDialog::reject);
    connect(ui->filterComboBox, &QComboBox::currentTextChanged, [&](QString text){
        emit comboBoxTextChange(ui->filterValueComboBox, text);
    });
    connect(ui->moreFilterComboBox, &QComboBox::currentTextChanged, [&](QString text){
        emit comboBoxTextChange(ui->moreFilterValueComboBox, text);
    });
    connect(ui->moreFilterComboBox_1, &QComboBox::currentTextChanged, [&](QString text){
        emit comboBoxTextChange(ui->moreFilterValueComboBox_1, text);
    });

    connect(this, &GwSortAndFilterDialog::comboBoxTextChange, this, &GwSortAndFilterDialog::onComboBoxTextChange);
}

void GwSortAndFilterDialog::onToggledAdvancePushButton(bool state)
{
    if(state == true){
        ui->advanceGroupBox->setVisible(true);
        ui->filterGroupBox_2->setVisible(true);
        ui->filterGroupBox_3->setVisible(true);
        ui->advancePushButton->setText("Collapse");
    }else{
        ui->advanceGroupBox->setVisible(false);
        ui->filterGroupBox_2->setVisible(false);
        ui->filterGroupBox_3->setVisible(false);
        ui->moreFilterComboBox->setCurrentText("Select Filter Field");
        ui->moreFilterComboBox_1->setCurrentText("Select Filter Field");
        ui->advanceSortFieldComboBox->setCurrentText("Select Sort Field");
        ui->advancePushButton->setText("Advance");
    }
}

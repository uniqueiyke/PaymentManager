#include "GwPaymentDialog.h"
#include "ui_GwPaymentDialog.h"
#include "GwGenaralFunctions.h"
#include <QMessageBox>
#include <QDebug>

GwPaymentDialog::GwPaymentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GwPaymentDialog)
{
    ui->setupUi(this);

    ui->NameLineEdit->setValidator(validators(stringRegExp("^[a-z ]+$"), this));
    ui->regNumLineEdit->setValidator(validators(stringRegExp("\\d{4}/ES\\d{3,4}/[NSP]S"), this));
    ui->dateEdit->setDate(QDate::currentDate());
    connect(ui->okPushButton, &QPushButton::clicked, this, &GwPaymentDialog::onClickOkButton);
    connect(ui->cancelPushButton, &QPushButton::clicked, this, &GwPaymentDialog::onClickCancelButton);
    connect(ui->schoolComboBox, &QComboBox::currentTextChanged, this, &GwPaymentDialog::onSchoolChange);
}

GwPaymentDialog::~GwPaymentDialog()
{
    delete ui;
}

QString GwPaymentDialog::regNumber() const
{
    return matchString(ui->regNumLineEdit->text(), stringRegExp("\\d{4}/ES\\d{3,4}/[NSP]S")).toUpper();
}

QString GwPaymentDialog::paidFor() const
{
    if("Select Payment Chioce" == ui->paymentComboBox->currentText())
        return "";
    return ui->paymentComboBox->currentText();
}

QString GwPaymentDialog::fullName() const
{
    return capitalizedEachWord(ui->NameLineEdit->text());
}

double GwPaymentDialog::amountPaid() const
{
    return ui->amountDoubleSpinBox->value();
}

QString GwPaymentDialog::studentClass() const
{
    if("Select Class" == ui->classComboBox->currentText())
        return "";
    return ui->classComboBox->currentText();
}

QString GwPaymentDialog::school() const
{
    if("Select School" == ui->schoolComboBox->currentText())
        return "";
    return ui->schoolComboBox->currentText();
}

QDate GwPaymentDialog::date() const
{
    return ui->dateEdit->date();
}

QString GwPaymentDialog::dateString() const
{
    return ui->dateEdit->text();
}

bool GwPaymentDialog::isPaymentMade() const
{
    return !(regNumber().isEmpty() || fullName().isEmpty() || paidFor().isEmpty() ||
             amountPaid() <= 0.0 || school().isEmpty() || studentClass().isEmpty() || dateString().isEmpty());
}

QMap<QString, QVariant> GwPaymentDialog::paymentParameters()
{
    QMap<QString, QVariant> paymentParams;
    paymentParams["regNum"] = QVariant(regNumber());
    paymentParams["name"] = QVariant(fullName());
    paymentParams["class"] = QVariant(studentClass());
    paymentParams["amount"] = QVariant(amountPaid());
    paymentParams["paymentFor"] = QVariant(paidFor());
    paymentParams["date"] = QVariant(dateString());

    return paymentParams;
}

void GwPaymentDialog::onClickOkButton()
{
    if(isPaymentMade()){
        emit paymentMade(regNumber(), fullName(), paidFor(), amountPaid());
        paymentParameters();
        accept();
        return;
    }

    QMessageBox::critical(nullptr, tr("Empty Field"), tr("Some of the information are"
                                          " not provided.\nComplete them and continue."));
    reject();
}

void GwPaymentDialog::onClickCancelButton()
{
    reject();
}

void GwPaymentDialog::onSchoolChange(const QString &school)
{
    QStringList nurseryClasses, primaryClasses, secondaryClasses;
    nurseryClasses << "Select Class" << "Day care" << "Pre-nursery" << "Nursery 1" << "Nursery 2" << "Nursery 3";
    primaryClasses << "Select Class" << "Primary 1" << "Primary 2" << "Primary 3" << "Primary 4" << "Primary 5" << "Primary 6";
    secondaryClasses << "Select Class" << "JSS 1" << "JSS 2" << "JSS 3" << "SS 1" << "SS 2" << "SS 3";

    if(school == "Nursery School"){
        ui->classComboBox->clear();
        ui->classComboBox->addItems(nurseryClasses);
    }else if (school == "Primary School") {
        ui->classComboBox->clear();
        ui->classComboBox->addItems(primaryClasses);
    }else if (school == "Secondary School"){
        ui->classComboBox->clear();
        ui->classComboBox->addItems(secondaryClasses);
    }else {
        ui->classComboBox->clear();
        ui->classComboBox->addItem("Select Class");
    }
}

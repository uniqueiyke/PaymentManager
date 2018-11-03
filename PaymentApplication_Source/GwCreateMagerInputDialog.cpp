#include "GwCreateMagerInputDialog.h"
#include "ui_GwCreateMagerInputDialog.h"
#include "GwGenaralFunctions.h"
#include <QMessageBox>
#include "GwDoubleDelegateForColumn.h"

GwCreateMagerInputDialog::GwCreateMagerInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GwCreateMagerInputDialog), delegate_(new GwDoubleDelegateForColumn)
{
    ui->setupUi(this);
    ui->payMentTableWidget->setFrameShape(QFrame::NoFrame);
    ui->payMentTableWidget->setItemDelegate(delegate_);
    setFixedSize(462,411);
    connect(ui->createPushButton, &QPushButton::clicked, this, &GwCreateMagerInputDialog::onClickCreateButton);
    connect(ui->cancelPushButton, &QPushButton::clicked, this,  &GwCreateMagerInputDialog::reject);
}

GwCreateMagerInputDialog::~GwCreateMagerInputDialog()
{
    delete ui;
    delete delegate_;
}

QString GwCreateMagerInputDialog::schoolName() const
{
    return ui->schoolComboBox->currentText();
}

QString GwCreateMagerInputDialog::term() const
{
    return ui->termComboBox->currentText();
}

QString GwCreateMagerInputDialog::year()
{
    auto str = ui->yearLineEdit->text();
    if(!isMatched(str, stringRegExp("\\d{4}/\\d{4}")))
        return "";
    QStringList l{str.split("/")};
    if(l[0].toInt() >= l[1].toInt()){
        QMessageBox::critical(this, tr("Value Invalid"), tr("The %1 academic Year is invalid").arg(str));
        return "";
    }
    return str.replace('/', '_');
}

QMap<QString, QVariant> GwCreateMagerInputDialog::paymenTypes()
{
    QMap<QString, QVariant> items;
    auto rowCount = ui->payMentTableWidget->rowCount();

    for(int r{}; r < rowCount; ++r){
        auto item1 = ui->payMentTableWidget->item(r, 0);
        auto item2 = ui->payMentTableWidget->item(r, 1);

        if(item1 && item2){
            items[toCamelCase(item1->text())] = QVariant(item2->text().toDouble());
        }
    }

    return items;
}

QString GwCreateMagerInputDialog::queryString()
{
    QString str;
    auto rowCount = ui->payMentTableWidget->rowCount();

    for(int r{}; r < rowCount; ++r){
        auto item1 = ui->payMentTableWidget->item(r, 0);

        if(item1){
            str += toCamelCase(item1->text()) + " DOUBLE NOT NULL, ";
        }
    }

    if(str.endsWith(", "))
        str.remove(str.size() - 2, 2);
    return str;
}

void GwCreateMagerInputDialog::onClickCreateButton()
{
    if(year().isEmpty()){
        QMessageBox::critical(this, tr("Empty Field"), tr("The Year field cannot be empty nor contain invalid value"));
        return reject();
    }

    accept();
}


#ifndef GWPAYMENTDIALOG_H
#define GwPAYMENTDIALOG_H

#include <QDialog>

namespace Ui {
class GwPaymentDialog;
}

class GwPaymentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GwPaymentDialog(QWidget *parent = 0);
    ~GwPaymentDialog();

    QString regNumber() const;
    QString paidFor() const;
    QString fullName() const;
    double amountPaid() const;
    QString studentClass() const;
    QString school() const;
    QDate date() const;
    QString dateString() const;

    bool isPaymentMade() const;

    QMap<QString, QVariant> paymentParameters();
private slots:
    void onClickOkButton();
    void onClickCancelButton();
    void onSchoolChange(const QString &school);

signals:
    void paymentMade(const QString &regNumber, const QString &fullName, const QString &paidFor, double amountPaid);
private:
    Ui::GwPaymentDialog *ui;
};

#endif // PAYMENTDIALOG_H

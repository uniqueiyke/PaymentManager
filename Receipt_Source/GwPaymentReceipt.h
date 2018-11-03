#ifndef GWPAYMENTRECEIPT_H
#define GWPAYMENTRECEIPT_H

#include <QMainWindow>
#include <QTextCursor>

#include "GwArguments.h"

class QPrinter;
class QTextEdit;

class GwPaymentReceipt : public QMainWindow
{
    Q_OBJECT
public:
    explicit GwPaymentReceipt(const GwArguments &arguments = GwArguments(), QWidget *parent = nullptr);
    explicit GwPaymentReceipt(const QString &fileName, QWidget *parent = nullptr);

    QString regNum() const;
    void setRegNum(const QString &regNum);
    QString fullName() const;
    void setFullName(const QString &fullName);
    double amountPaid() const;
    void setAmountPaid(double amountPaid);
    QString paymentFor() const;
    void setPaymentFor(const QString &paymentFor);

    QString schoolName() const;
    void setSchoolName(const QString &schoolName);

    QString studentClass() const;
    void setStudentClass(const QString &studentClass);

    QString date() const;
    void setDate(const QString &date);

    bool writeFile(const QString &fileName);
    bool readFile(const QString &fileName);

    GwArguments arguments() const;
    void setArguments(const GwArguments &arguments);

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void recieptReady();

public slots:
    void onPaymentMade(const QString &regNumber, const QString &fullName, const QString &paidFor, double amountPaid);

private slots:
    void onOpenFile();
    void onSaveFile();
    void onSaveFileAs();
    void onPrintPreview();
    void printPreview(QPrinter *printer);
    void filePrint();
    void filePrintPdf();
    void setCurrentFileName(const QString &fileName);
    bool maybeSave();

private:
    QString regNum_;
    QString fullName_;
    double amountPaid_;
    QTextCursor textCursor_;
    QString schoolName_;
    QString studentClass_;
    QString date_;
    QString fileName_;
    QString receiptPath_;

    QString paymentFor_;
    QTextEdit *textEdit_;
    GwArguments arguments_;

    void createReciept();
    void createMenuActions();
    void setupMember();

    void readSettings();
    void writeSettings();
};

#endif // GWPAYMENTRECEIPT_H

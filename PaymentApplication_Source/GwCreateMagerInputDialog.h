#ifndef GWCREATEMAGERINPUTDIALOG_H
#define GWCREATEMAGERINPUTDIALOG_H

#include <QDialog>
class GwDoubleDelegateForColumn;

namespace Ui {
class GwCreateMagerInputDialog;
}

class GwCreateMagerInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GwCreateMagerInputDialog(QWidget *parent = nullptr);
    ~GwCreateMagerInputDialog();

    QString schoolName() const;
    QString term() const;
    QString year();
    QMap<QString, QVariant> paymenTypes();

    QString queryString();

private slots:
    void onClickCreateButton();
private:
    Ui::GwCreateMagerInputDialog *ui;
    GwDoubleDelegateForColumn *delegate_;
};

#endif // GWCREATEMAGERINPUTDIALOG_H

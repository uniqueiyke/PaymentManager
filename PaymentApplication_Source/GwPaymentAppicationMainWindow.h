#ifndef GWPAYMENTAPPICATIONMAINWINDOW_H
#define GWPAYMENTAPPICATIONMAINWINDOW_H

#include <QMainWindow>

class GwTabWidget;
class GwSqlTableView;
class QPushButton;

namespace Ui {
class GwPaymentAppicationMainWindow;
}

class GwPaymentAppicationMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GwPaymentAppicationMainWindow(QWidget *parent = 0);
    ~GwPaymentAppicationMainWindow();

    bool createTable(const QString &paymentQueryStr, GwSqlTableView *tableView);
    void writeToFile(const QString &fileName, const QStringList &itemList);
    QStringList readFromFile();
protected:
    void closeEvent(QCloseEvent *event);

signals:
    void tableSorted();

private slots:
    void onTriggerMakePayment() const;
    void newPaymentManager();
    void openPaymentManager();
    void onTabBarOpened(bool value) const;
    void onClickEditability() const;
    void saveChangesMade() const;
    bool maybeSave(int index);
    bool maybeSave();
    void closeTabBar(int index);
    void onTriggerSortAndFilterACtion();
    void restoreBack();
    void setApplicationTitle();
    void onModelModification();
    void widgetModified();
    void modelContextMenu(int index);
    void onVHeaderContextMenuRequested(const QPoint &pos);
    void onTriggerAboutThisApp();

private:
    GwTabWidget *tabWidget_;
    Ui::GwPaymentAppicationMainWindow *ui;
    QPushButton *btn_;
    QList<int> modifiedTabs_;
};

#endif // GWPAYMENTAPPICATIONMAINWINDOW_H

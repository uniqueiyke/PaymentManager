#include "GwPaymentAppicationMainWindow.h"
#include "ui_GwPaymentAppicationMainWindow.h"
#include "GwTabWidget.h"
#include "GwPaymentDialog.h"
#include "GwSqlTableView.h"
#include "GwGenaralFunctions.h"
#include "GwCreateMagerInputDialog.h"
#include "GwSqlQueryModel.h"
#include "GwSortAndFilterDialog.h"

#include <QCloseEvent>
#include <QFileDialog>
#include <QInputDialog>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>
#include <QPushButton>
#include <QTimer>
#include <QSqlError>

GwPaymentAppicationMainWindow::GwPaymentAppicationMainWindow(QWidget *parent) :
    QMainWindow(parent), tabWidget_(new GwTabWidget(nullptr)),
    ui(new Ui::GwPaymentAppicationMainWindow), btn_(nullptr)
{
    ui->setupUi(this);
    setCentralWidget(tabWidget_);
    setApplicationTitle();
    ui->actionMakePayment->setVisible(false);
    ui->menuTransaction->setEnabled(false);
    ui->actionSaveChanges->setVisible(false);
    ui->actionSortAndFilterMode->setVisible(false);
    widgetModified();

    tabWidget_->disconnect(SIGNAL(tabCloseRequested(int))); //prevent the default closing of the tabs
    //This allows the singnal "tabCloseRequested(int)" to be connected to another closing funtion defined in this
    //class to change the closing behaviou

    connect(ui->actionMakePayment, &QAction::triggered, this, &GwPaymentAppicationMainWindow::onTriggerMakePayment);
    connect(ui->actionNewPaymentManager, &QAction::triggered, this, &GwPaymentAppicationMainWindow::newPaymentManager);
    connect(ui->actionOpenPaymentMaanager, &QAction::triggered, this, &GwPaymentAppicationMainWindow::openPaymentManager);
    connect(tabWidget_, &GwTabWidget::tabBarOpened, this, &GwPaymentAppicationMainWindow::onTabBarOpened);
    connect(ui->actionEditData, &QAction::triggered, this, &GwPaymentAppicationMainWindow::onClickEditability);
    connect(ui->actionSaveChanges, &QAction::triggered, this, &GwPaymentAppicationMainWindow::saveChangesMade);
    connect(tabWidget_, &GwTabWidget::tabCloseRequested, this, &GwPaymentAppicationMainWindow::closeTabBar);
    connect(ui->actionSortAndFilter, &QAction::triggered, this, &GwPaymentAppicationMainWindow::onTriggerSortAndFilterACtion);
    connect(this, &GwPaymentAppicationMainWindow::tableSorted, [&](){
        ui->actionSortAndFilterMode->setVisible(true); });
    connect(ui->actionSortAndFilterMode, &QAction::triggered, this, &GwPaymentAppicationMainWindow::restoreBack);
    connect(ui->actionAboutThisApp, &QAction::triggered, this, &GwPaymentAppicationMainWindow::onTriggerAboutThisApp);
    connect(ui->actionAboutQt, &QAction::triggered, [&](){qApp->aboutQt();});

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [&](){
        widgetModified();
        onModelModification();
    });
    timer->start(1000);

    connect(tabWidget_, &GwTabWidget::tabBarAdded, this, &GwPaymentAppicationMainWindow::modelContextMenu);
}

GwPaymentAppicationMainWindow::~GwPaymentAppicationMainWindow()
{
    delete ui;
}

bool GwPaymentAppicationMainWindow::createTable(const QString &paymentQueryStr, GwSqlTableView *tableView)
{
    tableView->dbCreator().db().transaction();
    bool a = tableView->dbCreator().createTable(QString("CREATE TABLE paymentTypeT(dataID INTEGER PRIMARY KEY, %1)").arg(paymentQueryStr) );

    bool b = tableView->dbCreator().createTable("CREATE TABLE paymentT(dataID INTEGER PRIMARY KEY, regNum VARCHAR(20) UNIQUE NOT NULL, name VARCHAR(225) NOT NULL, "
                            "class VARCHAR(20) NOT NULL, amount DOUBLE  NOT NULL, paymentFor VARCHAR(50) NOT NULL, "
                            "date VARCHAR(50) NOT NULL)");

    if(a && b && tableView->dbCreator().db().commit()){
        QMessageBox::information(this, QString(tr("Success")), QString(tr("Manager added")));
    }else{
        QMessageBox::critical(this, QString(tr("Failed")), QString(tr("Could not create Manager")));
        tableView->dbCreator().db().rollback();
        return false;
    }
    return true;
}

void GwPaymentAppicationMainWindow::writeToFile(const QString &fileName, const QStringList &itemList)
{
    QFile file(fileName);
    if(fileName.isEmpty()){return;}
    if(!file.open(QFile::WriteOnly)){
        QMessageBox::critical(this, tr("File Error"), tr("file could not open\nFile writing operation failed"));
    }

    QDataStream sOut(&file);
    sOut.setVersion(QDataStream::Qt_5_9);
    sOut << itemList;
}

QStringList GwPaymentAppicationMainWindow::readFromFile()
{
    auto fileName =  QFileDialog::getOpenFileName(this, tr("Open Manager"), projectRootPath(), tr("Payment Manager (*.pmg)"));

    if(fileName.isEmpty()){return QStringList();}
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly)){
        QMessageBox::critical(this, tr("File Error"), tr("file could not open\nFile open operation failed"));
    }

    QStringList itemList;

    QDataStream sIn(&file);
    sIn.setVersion(QDataStream::Qt_5_9);
    sIn >> itemList;

    return itemList;
}

void GwPaymentAppicationMainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void GwPaymentAppicationMainWindow::newPaymentManager()
{
    GwCreateMagerInputDialog dlg;
    int ret = dlg.exec();

    if(1 == ret){
        auto schoolName = dlg.schoolName();
        auto dbName = schoolName + "/" + dlg.year() + "/" + dlg.term();
        auto dbPath = projectRootPath(dbName);
        auto *tableView = new GwSqlTableView;
        auto dbFilePath = QString("%1/database/paymentManager.db").arg(dbPath);

        tableView->dbCreator().dbConnection(dbFilePath);
        tableView->dbCreator().db().transaction();
        tableView->reSetTableModel();
        createTable(dlg.queryString(), tableView);
        tableView->dbCreator().insertData("paymentTypeT", dlg.paymenTypes());
        tableView->refresh("paymentT");
        tableView->dbCreator().db().commit();
        tabWidget_->addTab(tableView, QString("%1 Payment").arg(schoolName));
        writeToFile(dbPath + QString("/%1.pmg").arg(schoolName.replace(" ", "_")), QStringList{schoolName, dbFilePath});
        QDir dir(dbPath + "/Reciepts");
        dir.mkpath(dbPath + "/Reciepts");
    }
}

void GwPaymentAppicationMainWindow::openPaymentManager()
{
    auto fileNames = readFromFile();
    if(fileNames.isEmpty()){return;}
    auto *tableView = new GwSqlTableView;
    tableView->dbCreator().dbConnection(fileNames.at(1));
    tableView->reSetTableModel();
    tableView->refresh("paymentT");
    tabWidget_->addTab(tableView, QString("%1 Payment").arg(fileNames.at(0)));
}

void GwPaymentAppicationMainWindow::onTabBarOpened(bool value) const
{
    if(value){
        ui->actionMakePayment->setVisible(true);
        ui->menuTransaction->setEnabled(true);
        ui->actionSaveChanges->setVisible(true);
    }else {
        ui->actionMakePayment->setVisible(false);
        ui->menuTransaction->setEnabled(false);
        ui->actionSaveChanges->setVisible(false);
    }
}

void GwPaymentAppicationMainWindow::onClickEditability() const
{
    auto *widget = qobject_cast<GwSqlTableView *>(tabWidget_->currentWidget());
    if(!widget){ return; }
    widget->onEditabilityChange();
}

void GwPaymentAppicationMainWindow::saveChangesMade() const
{
    auto *widget = qobject_cast<GwSqlTableView *>(tabWidget_->currentWidget());
    if(!widget){ return; }
    widget->onSaveData();
    widget->setWindowModified(false);
}

bool GwPaymentAppicationMainWindow::maybeSave(int index)
{
    auto *widget = qobject_cast<GwSqlTableView *>(tabWidget_->widget(index));
    if(!widget){ return true; }

    if(widget->isDirty()){
        int ret = QMessageBox::question(this, tr("Changes Made") , tr("The changes made to this tabel is not save.\n"
                                    "Click Save to save the tabel"),
                              QMessageBox::Save | QMessageBox::Ignore | QMessageBox::Cancel);
        if(QMessageBox::Save == ret){
            return widget->onSaveData();
        }else if (QMessageBox::Cancel == ret) {
            return false;
        }
    }


    widget->onSaveData(false);
    return true;
}

bool GwPaymentAppicationMainWindow::maybeSave()
{
    for(int i : modifiedTabs_){
        auto *widget = qobject_cast<GwSqlTableView *>(tabWidget_->widget(i));
        if(!widget){ continue; }
        maybeSave(i);
    }
    return true;
}

void GwPaymentAppicationMainWindow::closeTabBar(int index)
{
    if (maybeSave(index)) {
        tabWidget_->closeTabBar(index);
        if(modifiedTabs_.contains(index)){
            modifiedTabs_.removeOne(index);
        }
    }
}

void GwPaymentAppicationMainWindow::onTriggerSortAndFilterACtion()
{
    auto *widget = qobject_cast<GwSqlTableView *>(tabWidget_->currentWidget());

    GwSortAndFilterDialog dlg(widget->tableModel(), this);
    dlg.setFieldBoxItems();
    int ret = dlg.exec();

    if(QDialog::Accepted == ret){
        QSqlQuery query = widget->dbCreator().sortAndFilter("paymentT", dlg.tableSortOrder(), dlg.tableFilters());
        widget->tableModel()->setQuery(query);
        statusBar()->addAction(ui->actionSortAndFilterMode);
        btn_ = new QPushButton("sort and fiter mode");
        btn_->setIcon(QIcon(":/images/Filter.png"));
        connect(btn_, &QPushButton::clicked, this, &GwPaymentAppicationMainWindow::restoreBack);
        statusBar()->addPermanentWidget(btn_);
        emit tableSorted();
    }
}

void GwPaymentAppicationMainWindow::restoreBack()
{
    auto *widget = qobject_cast<GwSqlTableView *>(tabWidget_->currentWidget());
    widget->refresh("paymentT");
    ui->actionSortAndFilterMode->setVisible(false);
    delete btn_;
    btn_ = nullptr;
}

void GwPaymentAppicationMainWindow::setApplicationTitle()
{
    setWindowTitle(tr("%1[*]").arg(QCoreApplication::applicationName()));
    setWindowModified(false);
}

void GwPaymentAppicationMainWindow::onModelModification()
{
    auto *widget = qobject_cast<GwSqlTableView *>(tabWidget_->currentWidget());
    if(widget){
        auto index = tabWidget_->indexOf(widget);
        if(widget->isDirty() && !modifiedTabs_.contains(index)){
            modifiedTabs_ << index;
        }else if(!widget->isDirty() && modifiedTabs_.contains(index)){
            modifiedTabs_.removeOne(index);
        }
    }
}

void GwPaymentAppicationMainWindow::widgetModified()
{
    auto size = modifiedTabs_.size();
    if(size > 0){
        for(int i : modifiedTabs_){
            if(tabWidget_->isModified(i)){
                setWindowModified(true);
                break;
            }
        }
    }else {
        setWindowModified(false);
    }
}

void GwPaymentAppicationMainWindow::modelContextMenu(int index)
{
    auto *widget = qobject_cast<GwSqlTableView *>(tabWidget_->widget(index));
    if(!widget) return;
    widget->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(widget->verticalHeader(), &QHeaderView::customContextMenuRequested, this, &GwPaymentAppicationMainWindow::onVHeaderContextMenuRequested);
}

void GwPaymentAppicationMainWindow::onVHeaderContextMenuRequested(const QPoint &pos)
{
    auto *widget = qobject_cast<GwSqlTableView *>(tabWidget_->currentWidget());
    if(!widget) return;

    QMenu menu(this);
    menu.setStyleSheet("background-color: rgb(236, 236, 236);\ncolor: rgb(0, 0, 0)");

    QModelIndex index = widget->verticalHeader()->currentIndex();
    QSqlRecord record;
    if(index.isValid()){
        record = widget->tableModel()->record(index.row());
    }

    QString name = record.value("name").toString();

    auto *receiptAction = new QAction(tr("Create receipt for %1").arg(name), this);
    auto *deleteAction = new QAction(tr("Delete %1's record").arg(name), this);

    connect(receiptAction, &QAction::triggered,
            //Using a lambada funtion as a slot receipt action
            [&](){ //Lambada Function begins
        auto regNum = record.value("regNum").toString();
        auto fullName = record.value("name").toString();
        auto studentClass = record.value("class").toString();
        auto amountPaid = record.value("amount").toString();
        auto paidFor = record.value("paymentFor").toString();
        auto dateString = record.value("date").toString();

        QString school;

        if(regNum.endsWith("NS", Qt::CaseInsensitive)){
            school = "Nursery School";
        }else if(regNum.endsWith("PS", Qt::CaseInsensitive)){
            school = "Primary School";
        }else{
            school = "Secondary School";
        }

        auto program = QDir::currentPath() + "/GwReceipt.exe";
        QStringList arguments;
        arguments << regNum << fullName << school << studentClass << amountPaid << paidFor << dateString;
        QProcess::startDetached(program, arguments);

    });// Lambada Function ends

    connect(deleteAction, &QAction::triggered,
            //Using a lambada funtion as a slot
            [&](){ //Lambada Function begins
        QString regNum = record.value("regNum").toString();
        GwWhereClause regNumWhere("regNum", QVariant(regNum));

        QMessageBox::StandardButton ret = QMessageBox::question(this, tr("Delete Record"), tr("You are about to delete record of %1 %2 with Reg. Number %3.\n"
                                                            "Do you wish to continue with the delete operation?")
                   .arg(record.value("surname").toString()).arg(record.value("firstName").toString()).arg(regNum));
        if(ret == QMessageBox::Yes){
            widget->dbCreator().db().transaction();
            bool a = widget->dbCreator().deleteRecord("paymentT", regNumWhere);
            if(a && widget->dbCreator().db().commit()){
                QMessageBox::information(this, tr("Delete Record"), tr("Record Deleted"));
                widget->refresh("paymentT");
            }else{
                widget->dbCreator().db().rollback();
                QMessageBox::critical(this, tr("Delete Record"), tr("Record not Deleted %1").arg(widget->dbCreator().db().lastError().text()));
            }
        }
    });// Lambada Function ends

    menu.addAction(receiptAction);
    menu.addAction(deleteAction);
    menu.exec(widget->verticalHeader()->viewport()->mapToGlobal(pos));
}

void GwPaymentAppicationMainWindow::onTriggerAboutThisApp()
{
    QMessageBox::information(this, tr("About Registration Manager"), tr("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\">Version 1.0</span></p>"
                                                                        "<p>This application is use strictly for payment of fees of students in Nursery, Primary and Secondary Schools.\n"
                                                                        " It stores the students' payment details in an SQLite database.</p></body></html>"));
}

void GwPaymentAppicationMainWindow::onTriggerMakePayment() const
{
    GwPaymentDialog dlg;
    int ret = dlg.exec();

    if(ret == 1){
        auto *widget = qobject_cast<GwSqlTableView *>(tabWidget_->currentWidget());
        if(!widget){ return; }
        widget->dbCreator().insertData("paymentT", dlg.paymentParameters());
        widget->refresh("paymentT");

        auto program = QDir::currentPath() + "/GwReceipt.exe";
        QStringList arguments;
        arguments << dlg.regNumber() << dlg.fullName() << dlg.school() << dlg.studentClass() << QString::number(dlg.amountPaid()) << dlg.paidFor() << dlg.dateString();
        QProcess::startDetached(program, arguments);
    }
}


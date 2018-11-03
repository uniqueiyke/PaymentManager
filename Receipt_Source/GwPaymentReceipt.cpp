#include "GwPaymentReceipt.h"
#include <QTextEdit>
#include <QTextFrame>
#include <QDebug>

#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QFileDialog>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QCoreApplication>
#include <QSettings>

GwPaymentReceipt::GwPaymentReceipt(const GwArguments &arguments, QWidget *parent)
    : QMainWindow(parent), arguments_(arguments)
{
    textEdit_ = new QTextEdit;
    setCentralWidget(textEdit_);
    QRect rec =  geometry();
    setGeometry(rec.x() + 100, rec.y() + 100, 500, 300);
    textCursor_ = textEdit_->textCursor();

    setCurrentFileName("");

    createMenuActions();

    readSettings();
    setupMember();
}

GwPaymentReceipt::GwPaymentReceipt(const QString &fileName, QWidget *parent)
    : QMainWindow(parent)
{
    textEdit_ = new QTextEdit;
    setCentralWidget(textEdit_);
    QRect rec =  geometry();
    setGeometry(rec.x() + 100, rec.y() + 100, 500, 300);
    textCursor_ = textEdit_->textCursor();

    setCurrentFileName("");

    createMenuActions();

    readFile(fileName);
    readSettings();
}

QString GwPaymentReceipt::regNum() const
{
    return regNum_;
}

void GwPaymentReceipt::setRegNum(const QString &regNum)
{
    regNum_ = regNum;
}

QString GwPaymentReceipt::fullName() const
{
    return fullName_;
}

void GwPaymentReceipt::setFullName(const QString &fullName)
{
    fullName_ = fullName;
}

double GwPaymentReceipt::amountPaid() const
{
    return amountPaid_;
}

void GwPaymentReceipt::setAmountPaid(double amountPaid)
{
    amountPaid_ = amountPaid;
}

QString GwPaymentReceipt::paymentFor() const
{
    return paymentFor_;
}

void GwPaymentReceipt::setPaymentFor(const QString &paymentFor)
{
    paymentFor_ = paymentFor;
}

QString GwPaymentReceipt::schoolName() const
{
    return schoolName_;
}

void GwPaymentReceipt::setSchoolName(const QString &schoolName)
{
    schoolName_ = schoolName;
}

void GwPaymentReceipt::onPaymentMade(const QString &regNumber, const QString &fullName, const QString &paidFor, double amountPaid)
{
    setRegNum(regNumber);
    setFullName(fullName);
    setPaymentFor(paidFor);
    setAmountPaid(amountPaid);

    createReciept();
}

void GwPaymentReceipt::onOpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), receiptPath_ , tr("Receipt (*.recp *.rcp)"));
    if(fileName.isEmpty()){return;}
    if(readFile(fileName)){
        setCurrentFileName(fileName);
    }

    QFileInfo filePath(fileName);
    receiptPath_ = filePath.path();
}

void GwPaymentReceipt::onSaveFile()
{
    if(fileName_.isEmpty()){
        onSaveFileAs();
        return;
    }

    writeFile(fileName_);
    textEdit_->document()->setModified(false);
}

void GwPaymentReceipt::onSaveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), receiptPath_, tr("Receipt (*.recp *.rcp)"));
    if(fileName.isEmpty()){return;}
    if(writeFile(fileName)){
        fileName_ = fileName;
        setCurrentFileName(fileName);
    }

    QFileInfo filePath(fileName);
    receiptPath_ = filePath.path();
}

void GwPaymentReceipt::onPrintPreview()
{
#if QT_CONFIG(printpreviewdialog)
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, &QPrintPreviewDialog::paintRequested, this, printPreview);
    preview.exec();
#endif
}

void GwPaymentReceipt::printPreview(QPrinter *printer)
{
#ifdef QT_NO_PRINTER
    Q_UNUSED(printer);
#else
    textEdit_->print(printer);
#endif
}

void GwPaymentReceipt::filePrint()
{
#if QT_CONFIG(printdialog)
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (textEdit_->textCursor().hasSelection())
        dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    dlg->setWindowTitle(tr("Print Receipt"));
    if (dlg->exec() == QDialog::Accepted)
        textEdit_->print(&printer);
    delete dlg;
#endif
}

void GwPaymentReceipt::filePrintPdf()
{
#ifndef QT_NO_PRINTER
    QFileDialog fileDialog(this, tr("Export PDF"));
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setMimeTypeFilters(QStringList("application/pdf"));
    fileDialog.setDefaultSuffix("pdf");
    if (fileDialog.exec() != QDialog::Accepted)
        return;
    QString fileName = fileDialog.selectedFiles().first();
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    textEdit_->document()->print(&printer);
    statusBar()->showMessage(tr("Exported \"%1\"")
                             .arg(QDir::toNativeSeparators(fileName)));
#endif
}

void GwPaymentReceipt::setCurrentFileName(const QString &fileName)
{
    fileName_ = fileName;
    textEdit_->document()->setModified(false);

    QString shownName;
    if (fileName.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = QFileInfo(fileName).fileName();

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(QCoreApplication::applicationName()));
    setWindowModified(false);
}

bool GwPaymentReceipt::maybeSave()
{
    if(textEdit_->document()->isModified()){
        int ret = QMessageBox::question(this, tr("Document not save") , tr("The changes made to this document is not save.\n"
                                    "Click Save to save the document"),
                              QMessageBox::Save | QMessageBox::Ignore | QMessageBox::Cancel);
        if(QMessageBox::Save == ret){
            onSaveFile();
            return true;
        }else if (QMessageBox::Cancel == ret) {
            return false;
        }
    }

    return true;
}

GwArguments GwPaymentReceipt::arguments() const
{
    return arguments_;
}

void GwPaymentReceipt::setArguments(const GwArguments &arguments)
{
    arguments_ = arguments;
}

void GwPaymentReceipt::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

QString GwPaymentReceipt::date() const
{
    return date_;
}

void GwPaymentReceipt::setDate(const QString &date)
{
    date_ = date;
}

bool GwPaymentReceipt::writeFile(const QString &fileName)
{
    if(fileName.isEmpty()){return false;}
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly)){
        QMessageBox::critical(nullptr, tr("Open File Error"), tr("File cannot open"));
        return false;
    }

    QDataStream sOut(&file);
    sOut.setVersion(QDataStream::Qt_5_9);
    sOut << textEdit_->toHtml();

    file.close();
    file.flush();

    setWindowModified(false);
    return true;

}

bool GwPaymentReceipt::readFile(const QString &fileName)
{
    if(fileName.isEmpty()){return false;}
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly)){
        QMessageBox::critical(nullptr, tr("Open File Error"), tr("File cannot open"));
        return false;
    }

    QString str;
    QDataStream sIn(&file);
    sIn.setVersion(QDataStream::Qt_5_9);
    sIn >> str;

    file.close();

    if(!textEdit_->document()->isEmpty()){
        QPoint h = pos() + QPoint(30, 30);;

        auto *newWindow = new GwPaymentReceipt;
        newWindow->move(h);
        newWindow->textEdit_->setHtml(str);
        newWindow->show();
        newWindow->setWindowModified(false);

        return true;
    }
    textEdit_->setHtml(str);
    setWindowModified(false);
    return true;

}

QString GwPaymentReceipt::studentClass() const
{
    return studentClass_;
}

void GwPaymentReceipt::setStudentClass(const QString &studentClass)
{
    studentClass_ = studentClass;
}

void GwPaymentReceipt::createReciept()
{
    textCursor_.movePosition(QTextCursor::Start);

    QTextFrame *topFrame = textCursor_.currentFrame();
    QTextFrameFormat topFrameFormat = topFrame->frameFormat();
    topFrameFormat.setLeftMargin(20);
    topFrameFormat.setRightMargin(20);
    topFrameFormat.setTopMargin(10);
    topFrameFormat.setBottomMargin(10);
    topFrame->setFrameFormat(topFrameFormat);

    QTextBlockFormat blockFormat_;
    blockFormat_.setAlignment(Qt::AlignCenter);

    textCursor_.insertBlock(blockFormat_);

    QTextCharFormat defaultFormat = textCursor_.blockCharFormat();

    QTextCharFormat charFormat;
    charFormat.setFontFamily("Algerian");
    charFormat.setFontCapitalization(QFont::AllUppercase);
    charFormat.setFontPointSize(24);
    charFormat.setForeground(QBrush(QColor(0, 50, 0)));

    textCursor_.insertText(tr("Emak God's Own %1").arg(schoolName()), charFormat);

    QTextCharFormat tempFormat = defaultFormat;
    tempFormat.setForeground(QBrush(QColor(Qt::white)));
    tempFormat.setFontPointSize(14);
    tempFormat.setFontItalic(true);
    tempFormat.setBackground(Qt::black);

    QTextCharFormat headerFormat = defaultFormat;
    headerFormat.setFontPointSize(15);
    headerFormat.setFontWeight(1);

    QTextCharFormat textFormat = defaultFormat;
    textFormat.setFontPointSize(18);
    textFormat.setFontItalic(true);
    textFormat.setForeground(Qt::darkBlue);
    textFormat.setFontCapitalization(QFont::AllUppercase);


    textCursor_.insertBlock();
    textCursor_.insertText(tr("Temporary Receipt"), tempFormat);

    QTextBlockFormat fmt = blockFormat_;
    fmt.setAlignment(Qt::AlignLeft);

    textCursor_.insertBlock(fmt);
    textCursor_.insertText(tr("Reg. Number:  "), headerFormat);
    textCursor_.insertText(tr("%1").arg(regNum()), textFormat);

    textCursor_.insertBlock();
    textCursor_.insertText(tr("Name:  "), headerFormat);
    textCursor_.insertText(tr("%1").arg(fullName()), textFormat);

    textCursor_.insertBlock();
    textCursor_.insertText(tr("Class:  "), headerFormat);
    textCursor_.insertText(tr("%1").arg(studentClass()), textFormat);

    textCursor_.insertBlock();
    textCursor_.insertText(tr("Amount Paid:  "), headerFormat);
    textCursor_.insertText(tr("#%1").arg(amountPaid()), textFormat);

    textCursor_.insertBlock();
    textCursor_.insertText(tr("Paid For:  "), headerFormat);
    textCursor_.insertText(tr("%1").arg(paymentFor()), textFormat);

    textCursor_.insertBlock();
    textCursor_.insertText(tr("Date:  "), headerFormat);
    textCursor_.insertText(tr("%1").arg(date()), textFormat);

    textCursor_.insertBlock();
    textCursor_.insertText(tr("Signed: .................................  "), headerFormat);

}

void GwPaymentReceipt::createMenuActions()
{
    auto *openAction = new QAction("&Open");
    openAction->setShortcut(QKeySequence::Open);
    openAction->setToolTip("Open a receipt file");

    auto *saveAction = new QAction("&Save");
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setToolTip("Save the receipt to file");

    auto *saveAsAction = new QAction("S&ave As");
    saveAsAction->setShortcut(QKeySequence("ctrl+shift+S"));
    saveAsAction->setToolTip("Save as new receipt");


    auto *printAction = new QAction(tr("&Print"));
    printAction->setShortcut(QKeySequence::Print);
    printAction->setToolTip("Print the receipt");

    auto *printPreviewAction = new QAction(tr("Print Preview"));
    printPreviewAction->setShortcut(QKeySequence("ctrl+P+V"));
    printPreviewAction->setToolTip(tr("Preview the document before printing"));

    auto *printPdfAction = new QAction(tr("Print PDF"));
    printPdfAction->setShortcut(QKeySequence(tr("ctrl+shift+P")));
    printPdfAction->setToolTip("Export Document as PDF");

    auto *menu = menuBar()->addMenu(tr("&File"));
    menu->addActions(QList<QAction *>{openAction, saveAction, saveAsAction});
    menu->addSeparator();
    menu->addActions(QList<QAction *>{printAction, printPreviewAction, printPdfAction});

    connect(openAction, QAction::triggered, this, &GwPaymentReceipt::onOpenFile);
    connect(saveAction, &QAction::triggered, this, &GwPaymentReceipt::onSaveFile);
    connect(saveAsAction, &QAction::triggered, this, &GwPaymentReceipt::onSaveFileAs);
    connect(printAction, &QAction::triggered, this, &GwPaymentReceipt::filePrint);
    connect(printPdfAction, &QAction::triggered, this, &GwPaymentReceipt::filePrintPdf);
    connect(printPreviewAction, &QAction::triggered, this, &GwPaymentReceipt::onPrintPreview);
    connect(textEdit_->document(), &QTextDocument::modificationChanged, [&](){setWindowModified(textEdit_->document()->isModified());});
}

void GwPaymentReceipt::setupMember()
{
    if(arguments().argc() < 8){
        return;
    }


    char **argv = arguments().argv();
    setRegNum(argv[1]);
    setFullName(argv[2]);
    setSchoolName(argv[3]);
    setStudentClass(argv[4]);
    setAmountPaid(QString(argv[5]).toDouble());
    setPaymentFor(argv[6]);
    setDate(argv[7]);

    createReciept();
}

void GwPaymentReceipt::readSettings()
{
    QSettings settings;
    receiptPath_ = settings.value("receiptPath", QDir::homePath() + "/Payment Manager").toString();
}

void GwPaymentReceipt::writeSettings()
{
    QSettings settings;
    settings.setValue("receiptPath", receiptPath_);
}

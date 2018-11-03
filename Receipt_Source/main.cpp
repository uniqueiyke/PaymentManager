#include <QApplication>
#include <QCommandLineParser>
#include "GwPaymentReceipt.h"
#include <QMessageBox>

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("Payment Reciept");
    QCoreApplication::setApplicationVersion("Version 1.0");
    QCoreApplication::setOrganizationName("Grand Word Technology");

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file(s) to open.");
    parser.process(a);

    GwPaymentReceipt *mainWindow = Q_NULLPTR;
    auto args = parser.positionalArguments();
    if(args.size() == 1){
        mainWindow = new GwPaymentReceipt(args.first());
    }else{
        mainWindow = new GwPaymentReceipt(GwArguments(argc, argv));
    }

    mainWindow->show();

    return a.exec();
}

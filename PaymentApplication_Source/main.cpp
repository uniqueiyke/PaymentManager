#include "GwPaymentAppicationMainWindow.h"
#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("Payment Manager");
    QCoreApplication::setOrganizationName("Grand World Technology");
    QCoreApplication::setApplicationVersion("1.0");
    QCommandLineParser parser;
    parser.setApplicationDescription(QString("%1\nThis application is use strictly for payment"
                        " of fees in Nursery, Primary and Secondary Schools.\n"
                       "It stores the students' payment details in an SQLite"
                       " database").arg(QCoreApplication::applicationName()));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file(s) to open.");
    parser.process(a);
    GwPaymentAppicationMainWindow w;
    w.show();

    return a.exec();
}

#-------------------------------------------------
#
# Project created by QtCreator 2018-09-14T07:56:05
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
qtHaveModule(printsupport): QT += printsupport

TARGET = GwPaymentApplication
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        GwPaymentAppicationMainWindow.cpp \
    GwDatabaseCreator.cpp \
    GwTabWidget.cpp \
    GwSqlClauses.cpp \
    GwPaymentDialog.cpp \
    GwGenaralFunctions.cpp \
    GwItemDelegate.cpp \
    GwSqlQueryModel.cpp \
    GwSqlTableView.cpp \
    GwCreateMagerInputDialog.cpp \
    GwDoubleDelegateForColumn.cpp \
    GwSortAndFilterDialog.cpp

HEADERS += \
        GwPaymentAppicationMainWindow.h \
    GwDatabaseCreator.h \
    GwTabWidget.h \
    GwSqlClauses.h \
    GwPaymentDialog.h \
    GwGenaralFunctions.h \
    GwItemDelegate.h \
    GwSqlQueryModel.h \
    GwSqlTableView.h \
    GwCreateMagerInputDialog.h \
    GwDoubleDelegateForColumn.h \
    GwSortAndFilterDialog.h

FORMS += \
        GwPaymentAppicationMainWindow.ui \
    GwPaymentDialog.ui \
    GwCreateMagerInputDialog.ui \
    GwSortAndFilterDialog.ui

RESOURCES += \
    images.qrc

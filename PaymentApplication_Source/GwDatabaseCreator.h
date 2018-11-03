#ifndef GWDATABASECREATOR_H
#define GWDATABASECREATOR_H

#include <QString>
#include <QSqlDatabase>
#include <QDataStream>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlQuery>

#include "GwSqlClauses.h"


class GwDatabaseCreator
{
    friend QDataStream & operator<< (QDataStream& stream, const GwDatabaseCreator& dbCreator);
    friend QDataStream & operator>> (QDataStream& stream, GwDatabaseCreator& dbCreator);
    friend QDebug operator<< (QDebug stream, const GwDatabaseCreator& dbCreator);
public:

    enum DATATYPES {STRING, INT, LONG, DOUBLE};

    GwDatabaseCreator() = default;
    explicit GwDatabaseCreator(const QString &dbPath);

    void dbConnection(const QString &dbPath);

    QString dbErrors() const;
    void setDbErrors(const QString &error);
    bool isErrors() const;

    bool createTable(const QString &queryStr);

    void errorMessage(const QString &title, const QString &message);
    QStringList tables();
    QSqlDatabase db() const;
    QString dbName() const;

    QSqlQuery selectAll(const QString &tableName, const QString &order = "");
    QSqlQuery fetchData(const QString &tableName, const QString &whereColumnName, const QMap<DATATYPES, QVariant> &whereData);
    QSqlQuery fetchData(const QString &tableName, const GwWhereClause &whereClause = GwWhereClause());

    QVariant lastInsertedID() const;
    void setLastInsertedID(const QVariant &id);

    bool updateData(const QString &tableName, QMap<QString, QVariant> dataSet, const QString &regNum);
    bool updateData(const QString &tableName, QMap<QString, QVariant> dataSet, const QString &whereColumnName, const QMap<DATATYPES, QVariant> &whereData);
    bool updateData(const QString &tableName, const QMap<QString, QVariant> &dataSet, const GwWhereClause &whereClause);

    QSqlQuery queryE(const QString &queryStr);

    bool insertData(const QString &tableName, QMap<QString, QVariant> dataSet);
    bool deleteRecord(const QString &tableName, const QString &whereColumnName, const QMap<DATATYPES, QVariant> &whereData);
    bool deleteRecord(const QString &tableName, const GwWhereClause &whereClause = GwWhereClause());

    bool deleteTable(const QString &tableName);
    bool deleteDatabase(const QString &dbName);

    QSqlQuery sortAndFilter(const QString &tableName, const QList<GwWhereClause> &sortOrder, const QList<GwWhereClause> &filters);

private:
    QSqlDatabase dbh_;
    QString dbErrors_;
    QString dbName_;
    QVariant lastInsertedID_;
    QSqlQuery query_;

private:
    void resetErrorCheeker();
    bool connection(const QString &dbName, const QString &connStr);
    QString connectionName(const QString &conName); // generates a random unique name for the db connection

};
#endif // GWDATABASECREATOR_H

#include "GwDatabaseCreator.h"
#include <QtSql>
#include <QMessageBox>
#include <QDateTime>
#include <QFileInfo>

#include <ctime>
#include <cctype>

GwDatabaseCreator::GwDatabaseCreator(const QString &dbPath)
{dbConnection(dbPath);}

void GwDatabaseCreator::dbConnection(const QString &dbPath)
{
    QFileInfo fileInfo = QFileInfo(dbPath);
    if(!fileInfo.exists()){
        QString pathl = fileInfo.absolutePath();
        QDir path1 = QDir(pathl);
        path1.mkpath(pathl);
    }
    QString connString = fileInfo.baseName();
    connection(dbPath, connString);
}

QString GwDatabaseCreator::dbErrors() const
{
    return dbErrors_;
}

void GwDatabaseCreator::setDbErrors(const QString &error)
{
    dbErrors_ = error;
}

bool GwDatabaseCreator::isErrors() const
{
    if(dbErrors_.isEmpty()){
        return false;
    }
    return true;
}

bool GwDatabaseCreator::createTable(const QString &queryStr)
{
    if(!query_.exec(queryStr)){
        //errorMessage("Create Table Error", query_.lastError().text());
        qDebug() << query_.lastError().text();
        return false;
    }
    return true;
}

void GwDatabaseCreator::errorMessage(const QString &title, const QString &message)
{
    QMessageBox::critical(nullptr, QString("%1"), QString("%2").arg(title).arg(message));

}

QStringList GwDatabaseCreator::tables()
{
    return dbh_.tables();
}

QSqlDatabase GwDatabaseCreator::db() const
{
    return dbh_;
}

QString GwDatabaseCreator::dbName() const
{
    return dbName_;
}

QSqlQuery GwDatabaseCreator::selectAll(const QString &tableName, const QString &order)
{
    if(!order.isEmpty()){
        return QSqlQuery(QString("SELECT * FROM %1 ORDER BY regDate %2").arg(tableName).arg(order), db());
    }
    return QSqlQuery(QString("SELECT * FROM %1").arg(tableName), db());
}

QSqlQuery GwDatabaseCreator::fetchData(const QString &tableName, const QString &whereColumnName, const QMap<DATATYPES, QVariant> &whereData)
{
    //QSqlQuery query(db());
    QMap<DATATYPES, QVariant>::const_iterator itr = whereData.cbegin();
    switch(itr.key()){
        case DATATYPES::INT:
            query_.exec(QString("SELECT * FROM %1 WHERE %2 = %3").arg(tableName)
                       .arg(whereColumnName).arg(QString::number(itr.value().toInt())));
        break;
        case DATATYPES::LONG:
            query_.exec(QString("SELECT * FROM %1 WHERE %2 = %3").arg(tableName)
                       .arg(whereColumnName).arg(QString::number(itr.value().toLongLong())));
        break;
        case DATATYPES::DOUBLE:
            query_.exec(QString("SELECT * FROM %1 WHERE %2 = %3").arg(tableName)
                       .arg(whereColumnName).arg(QString::number(itr.value().toDouble())));
        break;
        default:
            query_.exec(QString("SELECT * FROM %1 WHERE %2 = \"%3\"").arg(tableName)
                       .arg(whereColumnName).arg(itr.value().toString()));
        break;
    }
    if(query_.lastError().isValid()){
        QMessageBox::critical(nullptr, QString("Data fetch Failed"),
                   QString("%1").arg(query_.lastError().text()));
    }
    return query_;
}

QSqlQuery GwDatabaseCreator::fetchData(const QString &tableName, const GwWhereClause &whereClause)
{
    if(whereClause.isEmpty()){
        return QSqlQuery(QString("SELECT * FROM %1").arg(tableName), db());
    }

    QString queryStr = QString("SELECT * FROM %1 WHERE %2 = :%2").arg(tableName).arg(whereClause.fieldName);
    //QSqlQuery query(db());
    query_.prepare(queryStr);
    query_.bindValue(QString(":%1").arg(whereClause.fieldName), whereClause.value);
    if(!query_.exec()){
        QMessageBox::critical(nullptr, QString("Data fetch Failed"),
                   QString("%1").arg(query_.lastError().text()));
    }

    return query_;
}

QVariant GwDatabaseCreator::lastInsertedID() const
{
    return lastInsertedID_;
}

void GwDatabaseCreator::setLastInsertedID(const QVariant &id)
{
    lastInsertedID_ = id;
}

bool GwDatabaseCreator::updateData(const QString &tableName, QMap<QString, QVariant> dataSet, const QString &regNum)
{
    QString str;
    QMap<QString, QVariant>::const_iterator it;
    QMap<QString, QVariant>::const_iterator end_it = dataSet.cend();

    for(it = dataSet.cbegin(); it != end_it; ++it){
        if(it.key() == "regNum" &&  it.value().toString()  == regNum){
            continue;
        }

        if(it == (end_it - 1)){
            str += it.key() + " = \"" + it.value().toString() + "\"";
        }
        else{
            str += it.key() + " = \"" + it.value().toString() + "\", ";
        }
    }


    QString queryStr = QString("UPDATE %1 SET %2 WHERE regNum = \"%3\"").arg(tableName).arg(str).arg(regNum);
    //QSqlQuery query(db());

    if(!query_.exec(queryStr)){
        QMessageBox::critical(nullptr, QString("Update Failed"),
                   QString("Error in %2:\n %1").arg(query_.lastError().text()).arg(tableName));
        return false;
    }
    return true;
}

bool GwDatabaseCreator::updateData(const QString &tableName, QMap<QString, QVariant> dataSet, const QString &whereColumnName, const QMap<GwDatabaseCreator::DATATYPES, QVariant> &whereData)
{
    QString str;
    QMap<QString, QVariant>::const_iterator it;
    QMap<QString, QVariant>::const_iterator end_it = dataSet.cend();

    for(it = dataSet.cbegin(); it != end_it; ++it){

        if(it == (end_it - 1)){
            str += it.key() + " = \"" + it.value().toString() + "\"";
        }
        else{
            str += it.key() + " = \"" + it.value().toString() + "\", ";
        }
    }

    QString queryStr;

    QMap<DATATYPES, QVariant>::const_iterator itr = whereData.cbegin();
    switch(itr.key()){
        case DATATYPES::INT:
            queryStr = QString("UPDATE %1 SET %2 WHERE %3 = %4").arg(tableName).arg(str)
                       .arg(whereColumnName).arg(QString::number(itr.value().toInt()));
            break;
        case DATATYPES::LONG:
            queryStr = QString("UPDATE %1 SET %2 WHERE %3 = %4").arg(tableName).arg(str)
                       .arg(whereColumnName).arg(QString::number(itr.value().toLongLong()));
            break;
        case DATATYPES::DOUBLE:
            queryStr = QString("UPDATE %1 SET %2 WHERE %3 = %4").arg(tableName).arg(str)
                       .arg(whereColumnName).arg(QString::number(itr.value().toDouble()));
            break;
        default:
            queryStr = QString("UPDATE %1 SET %2 WHERE %3 = \"%4\"").arg(tableName).arg(str)
                       .arg(whereColumnName).arg(itr.value().toString());
            break;
    }

    //QSqlQuery query(db());
    if(!query_.exec(queryStr)){
        QMessageBox::critical(nullptr, QString("Update Failed"),
                   QString("Error in %2:\n %1").arg(query_.lastError().text()).arg(tableName));
        return false;
    }
    return true;
}

bool GwDatabaseCreator::updateData(const QString &tableName, const QMap<QString, QVariant> &dataSet, const GwWhereClause &whereClause)
{
    QString strKey;

    {
        QMap<QString, QVariant>::const_iterator it;
        QMap<QString, QVariant>::const_iterator end_it = dataSet.cend();

        for(it = dataSet.cbegin(); it != end_it; ++it){
            if(it == (end_it - 1)){
                strKey += it.key() + " = :" + it.key();
            }
            else{
                strKey += it.key() + " = :" + it.key() + ", ";
            }
        }
    }

    QString field = whereClause.fieldName;
    QString queryStr = QString(" UPDATE %1 SET %2 WHERE %3 = :%3").arg(tableName).arg(strKey).arg(field);

    //qDebug() << queryStr;
    //QSqlQuery query(db());
    query_.prepare(queryStr);


    {
        QMap<QString, QVariant>::const_iterator it;
        QMap<QString, QVariant>::const_iterator end_it = dataSet.cend();

        for(it = dataSet.cbegin(); it != end_it; ++it){
            query_.bindValue(QString(":%1").arg(it.key()), it.value());
        }
    }

    query_.bindValue(QString(":").arg(field), whereClause.value);
    if(!query_.exec(queryStr)){
        QMessageBox::critical(nullptr, QString("Update Failed"),
                   QString("Error in %2:\n %1").arg(query_.lastError().text()).arg(tableName));
        return false;
    }
    return true;
}

QSqlQuery GwDatabaseCreator::queryE(const QString &queryStr)
{
    return QSqlQuery(queryStr, db());
}

bool GwDatabaseCreator::insertData(const QString &tableName, QMap<QString, QVariant> dataSet)
{
    QString strKey, strBindValue;

    {
        QMap<QString, QVariant>::const_iterator it;
        QMap<QString, QVariant>::const_iterator end_it = dataSet.cend();

        for(it = dataSet.cbegin(); it != end_it; ++it){
            if(it == (end_it - 1)){
                strKey += it.key();
                strBindValue += ":" + it.key();
            }
            else{
                strKey += it.key() + ", ";
                strBindValue += ":" + it.key() + ", ";
            }
        }
    }

    QString queryStr = QString("INSERT INTO %1(%2) "
                               "VALUES(%3)").arg(tableName).arg(strKey).arg(strBindValue);

    //QSqlQuery query(db());
    query_.prepare(queryStr);

    {
        QMap<QString, QVariant>::const_iterator it;
        QMap<QString, QVariant>::const_iterator end_it = dataSet.cend();

        for(it = dataSet.cbegin(); it != end_it; ++it){
            query_.bindValue(QString(":%1").arg(it.key()), it.value());
        }
    }


    if(!query_.exec()){
        QMessageBox::critical(nullptr, QString("Insert Failed"),
                   QString("Error in %2.\nThe is %1").arg(query_.lastError().text()).arg(tableName));
        return false;
    }

    setLastInsertedID(query_.lastInsertId());
    return true;
}

bool GwDatabaseCreator::deleteRecord(const QString &tableName, const QString &whereColumnName, const QMap<GwDatabaseCreator::DATATYPES, QVariant> &whereData)
{
    //QSqlQuery query(db());
    QMap<DATATYPES, QVariant>::const_iterator itr = whereData.cbegin();
    bool deleted = false;

    switch(itr.key()){
        case DATATYPES::INT:
            deleted = query_.exec(QString("DELETE FROM %1 WHERE %2 = %3").arg(tableName)
                       .arg(whereColumnName).arg(QString::number(itr.value().toInt())));
            break;
        case DATATYPES::LONG:
            deleted = query_.exec(QString("DELETE FROM %1 WHERE %2 = %3").arg(tableName)
                       .arg(whereColumnName).arg(QString::number(itr.value().toLongLong())));
            break;
        case DATATYPES::DOUBLE:
            deleted = query_.exec(QString("DELETE FROM %1 WHERE %2 = %3").arg(tableName)
                       .arg(whereColumnName).arg(QString::number(itr.value().toDouble())));
            break;
        default:
            deleted = query_.exec(QString("DELETE FROM %1 WHERE %2 = \"%3\"").arg(tableName)
                       .arg(whereColumnName).arg(itr.value().toString()));
            break;
    }

    if(!deleted){
        QMessageBox::critical(nullptr, QString("Data fetch Failed"),
                   QString("%1").arg(query_.lastError().text()));
        return false;
    }
    return true;
}

bool GwDatabaseCreator::deleteRecord(const QString &tableName, const GwWhereClause &whereClause)
{
    QString queryStr = QString("DELETE FROM %1 WHERE %2 = :%2").arg(tableName).arg(whereClause.fieldName);
    //QSqlQuery query(db());
    query_.prepare(queryStr);
    query_.bindValue(QString(":%1").arg(whereClause.fieldName), whereClause.value);

    if(!query_.exec()){
        QMessageBox::critical(nullptr, QString("Data fetch Failed"),
                   QString("%1").arg(query_.lastError().text()));
        return false;
    }
    return true;
}

bool GwDatabaseCreator::deleteTable(const QString &tableName)
{
    //QSqlQuery query(db());
    bool y = query_.exec(QString("DROP TABLE %1").arg(tableName));
    if(!y){
        return false;
    }

    return true;
}

bool GwDatabaseCreator::deleteDatabase(const QString &dbName)
{
    //QSqlQuery query(db());
    bool y = query_.exec(QString("DROP DATABASE %1").arg(dbName));
    if(!y){
        return false;
    }

    return true;
}

QSqlQuery GwDatabaseCreator::sortAndFilter(const QString &tableName, const QList<GwWhereClause> &sortOrder, const QList<GwWhereClause> &filters)
{
    QString filterStr;
    QString sortOrders;

    if(!filters.isEmpty()){
        auto size = filters.size();
        int count{};
        for(GwWhereClause item : filters){
            if(!item.fieldName.isEmpty() && !item.value.toString().isEmpty()){
                if(count == size - 1){
                    filterStr += item.fieldName + " = :" + item.fieldName;
                }else{
                    filterStr += item.fieldName + " = :" + item.fieldName + " OR ";
                }
            }
            count++;
        }
    }

    if(!sortOrder.isEmpty()){
        auto size = sortOrder.size();
        int count{};
        for(GwWhereClause item : sortOrder){
            if(!item.fieldName.isEmpty() && !item.value.toString().isEmpty()){
                if(count == size - 1){
                    sortOrders += item.fieldName + "  " + item.value.toString();
                }else{
                    sortOrders += item.fieldName + "  " + item.value.toString() + ", ";
                }
            }
            count++;
        }
    }

     QString queryStr ;

    if(!sortOrders.isEmpty() && !filterStr.isEmpty()){
        queryStr = QString("SELECT * FROM %1 WHERE %2 ORDER BY %3").arg(tableName)
                .arg(filterStr).arg(sortOrders);
    }else if(!sortOrders.isEmpty()){
        queryStr = QString("SELECT * FROM %1 ORDER BY %2").arg(tableName).arg(sortOrders);
    }else if(!filterStr.isEmpty()){
        queryStr = QString("SELECT * FROM %1 WHERE %2").arg(tableName).arg(filterStr);
    }else{
        queryStr = QString("SELECT * FROM %1").arg(tableName);
    }

    //QSqlQuery query(dbh_);
    query_.prepare(queryStr);

    if(!filterStr.isEmpty()){
        for(GwWhereClause item : filters){
            query_.bindValue(QString(":%1").arg(item.fieldName), item.value);
        }
    }

    if(!query_.exec()){
        QMessageBox::critical(nullptr, QString("Data fetch Failed"),
                   QString("%1").arg(query_.lastError().text()));
    }
    return query_;
}

void GwDatabaseCreator::resetErrorCheeker()
{
    dbErrors_ = QString();
}

bool GwDatabaseCreator::connection(const QString &dbName, const QString &connStr)
{
    QString conName = connectionName(connStr);
    QStringList connNames = QSqlDatabase::connectionNames();

    while(connNames.contains(conName, Qt::CaseInsensitive)){
        conName = connectionName(connStr);
    }

    dbh_ = QSqlDatabase::addDatabase("QSQLITE", conName);
    dbh_.setDatabaseName(dbName);
    dbName_ = dbName;

    if(!dbh_.open()){
        qDebug() << dbh_.lastError().text();
        return false;
    }

    query_ = dbh_.exec("PRAGMA foreign_keys = ON;");
    if(dbh_.lastError().isValid()){
        QMessageBox::StandardButton ret = QMessageBox::warning(nullptr, QString("Foreign Key"), QString("Foreign Key failed to turn ON\n%1.\n "
                 "Do you wish to continue without the foreign Constraint activiated?").arg(dbh_.lastError().text()));
        if(ret == QMessageBox::Yes){
            return true;
        }else{
            return false;
        }
    }
    return true;
}

QString GwDatabaseCreator::connectionName(const QString &conName)
{
    srand(time(0));
    int conNum = rand();
    return QString("connName_%1").arg(conName) + QString::number(conNum);
}

QDataStream &operator<<(QDataStream &stream, const GwDatabaseCreator &dbCreator)
{
    stream << dbCreator.dbName_;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, GwDatabaseCreator &dbCreator)
{
    stream >> dbCreator.dbName_;
    return stream;
}

QDebug operator<<(QDebug stream, const GwDatabaseCreator &dbCreator)
{
    stream << dbCreator.dbName_;
    return stream;
}

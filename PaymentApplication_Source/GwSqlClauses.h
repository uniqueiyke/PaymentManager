#ifndef GWSQLCLAUSES_H
#define GWSQLCLAUSES_H
#include <QString>
#include <QVariant>
#include <QDebug>

struct GwWhereClause{
    friend QDebug operator<< (QDebug stream, const GwWhereClause &where);

    explicit GwWhereClause(const QString &fieldName = "", const QVariant &value = QVariant());
    bool isEmpty() const{
        if(fieldName.isEmpty() || !value.isValid())
            return true;
        return false;
    }

    QString fieldName;
    QVariant value;
};

class GwSqlClauses
{
public:
    GwSqlClauses();
};

#endif // GWSQLCLAUSES_H

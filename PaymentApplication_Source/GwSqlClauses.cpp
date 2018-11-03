#include "GwSqlClauses.h"

GwSqlClauses::GwSqlClauses()
{

}

QDebug operator<<(QDebug stream, const GwWhereClause &where)
{
    stream << "GwWhereClause(" << where.fieldName << ", " << where.value << ")";
    return stream;
}

GwWhereClause::GwWhereClause(const QString &fieldName, const QVariant &value)
{
    this->fieldName = fieldName;
    this->value = value;
}

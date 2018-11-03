#ifndef GWGENARALFUNCTIONS_H
#define GWGENARALFUNCTIONS_H

#include <QRegularExpressionValidator>
#include <QDir>

class QComboBox;

enum class SCHOOL {NOSCHOOL, NURSERY, PRYMARY, SECONDARY};
QRegularExpression stringRegExp(const QString &str);

QString matchString(const QString &name, const QRegularExpression &regExp);
bool isMatched(const QString &name, const QRegularExpression &regExp);

QString capitalizedEachWord(const QString &string);

QStringList stateLGA(const QString &state);
QStringList statesInNigeria();

void onComboBoxTextChange(QComboBox * const comboBox, const QString &state);
QRegularExpressionValidator *validators(const QRegularExpression &re, QObject *parent);

QString projectRootPath(const QString &rootPath = "");

QString toCamelCase(const QString &str);
QString fromCamelCase(const QString &str);

#endif // GWGENARALFUNCTIONS_H

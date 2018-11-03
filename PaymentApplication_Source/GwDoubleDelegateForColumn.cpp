#include "GwDoubleDelegateForColumn.h"
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QRegularExpressionValidator>

GwDoubleDelegateForColumn::GwDoubleDelegateForColumn(QObject *parent) : QStyledItemDelegate(parent)
{}

QWidget *GwDoubleDelegateForColumn::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    if(index.column() == 1 ){
        auto *editor = new QDoubleSpinBox(parent);
        editor->setMinimum(0.00);
        editor->setMaximum(20000000);
        editor->setSingleStep(500.00);
        editor->setDecimals(2);
        editor->setFrame(false);
        return editor;
    }

    auto *editor = new QLineEdit(parent);
    editor->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-z ]*", QRegularExpression::CaseInsensitiveOption), parent));
    return editor;
}

void GwDoubleDelegateForColumn::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == 1){
        auto value = index.model()->data(index, Qt::EditRole);
        auto val = value.toDouble();
        auto *dSpinBox = static_cast<QDoubleSpinBox *>(editor);
        dSpinBox->setValue(val);
        return;
    }

    auto value = index.model()->data(index, Qt::EditRole);
    auto val = value.toString();
    auto *lineEdit = static_cast<QLineEdit *>(editor);
    lineEdit->setText(val);
}

void GwDoubleDelegateForColumn::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.column() == 1){
        auto *castedWidget = static_cast<QDoubleSpinBox*>(editor);
        castedWidget->interpretText();
        auto value = castedWidget->value();
        model->setData(index, value, Qt::EditRole);
        return;
    }

    auto *lineEdit = static_cast<QLineEdit*>(editor);
    auto value = lineEdit->text();
    model->setData(index, value, Qt::EditRole);
}

void GwDoubleDelegateForColumn::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

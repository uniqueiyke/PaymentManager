#include "GwItemDelegate.h"
#include <QSpinBox>
#include <QLineEdit>
#include <QDebug>
#include <QDateTimeEdit>
GwItemDelegate::GwItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
}

QWidget *GwItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto value = index.model()->data(index, Qt::EditRole);

    if(value.type() == QVariant::Int || value.type() == QMetaType::LongLong || value.type() == QMetaType::Long){
        auto *editor = new QSpinBox(parent);
        editor->setFrame(false);
        editor->setMinimum(INT_MIN);
        editor->setMaximum(INT_MAX);
        return editor;
    }

    if(value.type() == QMetaType::Double){
        auto *editor = new QDoubleSpinBox(parent);
        editor->setFrame(false);
        return editor;
    }

    if(value.type() == QMetaType::QDateTime){
            auto *editor = new QDateTimeEdit(parent);
            editor->setFrame(false);
            return editor;
        }
    if(value.type() == QMetaType::QString){
        auto *editor = new QLineEdit(parent);
        editor->setFrame(false);
        return editor;
    }
}

void GwItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto value = index.model()->data(index, Qt::EditRole);
    if(value.type() == QMetaType::Int || value.type() == QMetaType::LongLong || value.type() == QMetaType::Long){
        auto val = value.toInt();
        auto *spinBox = qobject_cast<QSpinBox*>(editor);
        spinBox->setValue(val);
    }else if(value.type() == QMetaType::Double){
        auto val = value.toDouble();
        auto *dSpinBox = qobject_cast<QDoubleSpinBox *>(editor);
        dSpinBox->setValue(val);
    }else if(value.type() == QMetaType::QDateTime){
        auto val = value.toDateTime();
        auto *dateTime = qobject_cast<QDateTimeEdit *>(editor);
        dateTime->setDateTime(val);
    }else{
        auto val = value.toString();
        auto *lineEdit = qobject_cast<QLineEdit *>(editor);
        lineEdit->setText(val);
    }
}

void GwItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    {
        auto *castedWidget = qobject_cast<QSpinBox*>(editor);
        if(castedWidget){
            castedWidget->interpretText();
            auto value = castedWidget->value();
            model->setData(index, value, Qt::EditRole);
            return;
        }
    }
    {
        auto *castedWidget = qobject_cast<QDoubleSpinBox*>(editor);
        if(castedWidget){
            castedWidget->interpretText();
            auto value = castedWidget->value();
            model->setData(index, value, Qt::EditRole);
            return;
        }
    }

    {
        auto *castedWidget = qobject_cast<QLineEdit*>(editor);
        if(castedWidget){
            auto value = castedWidget->text();
            model->setData(index, value, Qt::EditRole);
            return;
        }
    }
}

void GwItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);

}

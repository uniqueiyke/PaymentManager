#ifndef GWDOUBLEDELEGATEFORCOLUMN_H
#define GWDOUBLEDELEGATEFORCOLUMN_H

#include <QStyledItemDelegate>

class GwDoubleDelegateForColumn : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit GwDoubleDelegateForColumn(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

signals:

public slots:
};

#endif // GWDOUBLEDELEGATEFORCOLUMN_H

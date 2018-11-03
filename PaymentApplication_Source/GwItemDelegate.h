#ifndef GWITEMDELEGATE_H
#define GWITEMDELEGATE_H

#include <QStyledItemDelegate>

class GwItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit GwItemDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

signals:

public slots:

};

#endif // GWITEMDELEGATE_H

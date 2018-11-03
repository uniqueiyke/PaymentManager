#ifndef GWTABWIDGET_H
#define GWTABWIDGET_H

#include <QTabWidget>

class GwTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit GwTabWidget(QWidget *parent = nullptr);

    QStringList tabNamesList();
    bool contains(const QString &tab);
    int tabIndex(const QString &text);
    void removeTabLabel(const QString &text);
    bool isModified(int index);

signals:
    void tabBarOpened(bool value); // This signal is emited when a tabBar added
              // and when all the tabBar on the tabWidget is clossed

    void tabBarAdded(int index); // This signal is emited when a tabBar is added;


public slots:
    int addTab(QWidget *page, const QString &label);
    int addTab(QWidget *page, const QIcon &icon, const QString &label);
    virtual void closeTabBar(int index);

private:
    QStringList tabNamesList_;
};

#endif // GWTABWIDGET_H

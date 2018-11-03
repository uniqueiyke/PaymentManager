#include "GwTabWidget.h"
#include <QDebug>

GwTabWidget::GwTabWidget(QWidget *parent) : QTabWidget(parent)
{
    setTabsClosable(true);
    connect(this, &GwTabWidget::tabCloseRequested, this, &GwTabWidget::closeTabBar);
    connect(this, &GwTabWidget::tabBarAdded, this, &GwTabWidget::setCurrentIndex);
}

QStringList GwTabWidget::tabNamesList()
{
    return tabNamesList_;
}

bool GwTabWidget::contains(const QString &tab)
{
    return tabNamesList().contains(tab);
}

int GwTabWidget::tabIndex(const QString &text)
{
    auto list = tabNamesList();
    for(int i = 0, c = list.size(); i < c; ++i){
        if(text == tabText(i))
            return i;
    }
    return -1;
}

void GwTabWidget::removeTabLabel(const QString &text)
{
    if(tabNamesList_.contains(text)){
        tabNamesList_.removeOne(text);
    }
}

bool GwTabWidget::isModified(int index)
{
    return widget(index)->isWindowModified();
}

int GwTabWidget::addTab(QWidget *page, const QString &label)
{
    int index = QTabWidget::addTab(page, label);
    emit tabBarAdded(index);
    emit tabBarOpened(true);

    tabNamesList_ << label;

    return index;
}

int GwTabWidget::addTab(QWidget *page, const QIcon &icon, const QString &label)
{
    int index = QTabWidget::addTab(page, icon, label);
    emit tabBarAdded(index);
    emit tabBarOpened(true);

    tabNamesList_ << label;

    return index;
}

void GwTabWidget::closeTabBar(int index)
{
    removeTabLabel(tabText(index));
    delete widget(index);
    if(count() <= 0){
        emit tabBarOpened(false);
    }
}

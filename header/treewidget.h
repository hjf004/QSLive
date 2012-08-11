#ifndef TREEWIDGET_H
#define TREEWIDGET_H
#include<QtGui>
class ChannelList;
class TreeWidget:public QTreeWidget
{
    Q_OBJECT
public:
    friend class ChannelList;
    TreeWidget(QWidget *parent=0);
protected:
    void contextMenuEvent(QContextMenuEvent *);
private:
    QMenu *menu;
    QAction *view;
    QAction *add;
    QAction *remove;
    QAction *modifyName;
    QAction *modifyUrl;
};

#endif // TREEWIDGET_H

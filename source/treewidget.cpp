#include"header/treewidget.h"

TreeWidget::TreeWidget(QWidget *parent):QTreeWidget(parent)
{
    menu=new QMenu(this);
    view=new QAction(QObject::tr("&View Channel Url"),menu);
    add=new QAction(QObject::tr("&Add to Favorite Channel List"),menu);
    remove=new QAction(QObject::tr("&Remove From Channel List"),menu);
    modifyName=new QAction(QObject::tr("Modify Channel &Name"),menu);
    modifyUrl=new QAction(QObject::tr("Modify Channel &Url"),menu);
    menu->addAction(view);
    menu->addAction(add);
    menu->addAction(remove);
    menu->addAction(modifyName);
    menu->addAction(modifyUrl);
}

void TreeWidget::contextMenuEvent(QContextMenuEvent *e)
{
    menu->exec(QCursor::pos());
}

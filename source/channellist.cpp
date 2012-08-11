#include"header/channellist.h"
#include<sys/types.h>
#include<signal.h>
ChannelList::ChannelList(QString s,QWidget *parent):QWidget(parent)
{
    listUrl=QUrl(s);
    treeWidget=new TreeWidget(this);
    treeWidget->setHeaderLabel(QString());
    //    treeWidget->setRootIsDecorated( false );
    treeWidget->setFrameStyle(QFrame::NoFrame);
    infoLabel=new QLabel(QObject::tr("Channels List"));
    refreshButton=new QToolButton(this);
    refreshButton->setIcon(QIcon(":/resource/pic/refresh.png"));
    refreshButton->setToolTip(QObject::tr("Refresh Channel List."));
    QGroupBox *box=new QGroupBox(this);
    QHBoxLayout *hlayout=new QHBoxLayout(box);
    hlayout->addWidget(infoLabel);
    hlayout->addWidget(refreshButton);
    box->setLayout(hlayout);
    QVBoxLayout *vlayout=new QVBoxLayout(this);
    vlayout->addWidget(box);
    vlayout->addWidget(treeWidget);
    this->setLayout(vlayout);
    setMaximumWidth(330);
    RefreshList();

    connect(refreshButton,SIGNAL(clicked()),this,SLOT(RefreshList()));
    connect(treeWidget->view,SIGNAL(triggered()),this,SLOT(handleView()));
    connect(treeWidget->add,SIGNAL(triggered()),this,SLOT(handleAdd()));
    connect(treeWidget->remove,SIGNAL(triggered()),this,SLOT(handleRemove()));
    connect(treeWidget->modifyName,SIGNAL(triggered()),this,SLOT(handleModifyName()));
    connect(treeWidget->modifyUrl,SIGNAL(triggered()),this,SLOT(handleModifyUrl()));
}

void ChannelList::RefreshList()
{
    treeWidget->clear();
    emit listReady(false);
    treeWidget->setColumnCount(1);
    QTreeWidgetItem *item;
    item=new QTreeWidgetItem(treeWidget,QStringList(QObject::tr("Favorite Channels")));
    treeWidget->addTopLevelItem(item);
    readList();
    treeWidget->expandAll();
    QNetworkAccessManager *am=new QNetworkAccessManager();
    QNetworkRequest rq(listUrl);
    rp=am->get(rq);
    connect(rp,SIGNAL(finished()),this,SLOT(getList()));
    connect(this,SIGNAL(getFinished()),this,SLOT(handleInit()));
}

void ChannelList::getList()
{
    parser=new XmlParser(rp->readAll());
    emit getFinished();
}

void ChannelList::handleInit()
{
    channels=parser->getInfo();
    QMap<QString,QStringList>::iterator itr=channels.begin();
    QTreeWidgetItem *item;
    QTreeWidgetItem *child;
    for(;itr!=channels.end();itr++)
    {
        QStringList group(itr.key());
        item=new QTreeWidgetItem(treeWidget,group);
        treeWidget->addTopLevelItem(item);
        QStringList t=itr.value();
        qDebug()<<"Group "<<itr.key();
        for(int i=0;i<t.size()/2;i++)
        {
            child=new QTreeWidgetItem(item,QStringList(t.at(i*2)));
            item->addChild(child);
            allChannels.insert(t.at(i*2),t.at(i*2+1));
        }
        item->setExpanded(false);
    }
    item=treeWidget->topLevelItem(0);
    treeWidget->setCurrentItem(item);
    treeWidget->expandAll();
    emit listReady(true);
}

void ChannelList::setListUrl(QString s)
{
    listUrl=QUrl(s);
}

QString ChannelList::getListUrl()
{
    return listUrl.toString();
}

QTreeWidgetItem* ChannelList::addToList(QString name,QString url)
{
    QTreeWidgetItem *item=treeWidget->topLevelItem(0);
    QTreeWidgetItem *child=new QTreeWidgetItem(item,QStringList(name));
    item->addChild(child);
    allChannels.insert(name,url);
    return child;
}

void ChannelList::readList()
{
    QString list(QDir::homePath().append("/.config/QSLive/channel"));
    if(QFile::exists(list))
    {
        QFile f(list);
        if(f.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QTextStream in(&f);
            int i;
            while(!in.atEnd())
            {
                QString s(in.readLine());
                i=s.indexOf('=');
                QString channel=s.left(i).trimmed();
                QString url=s.mid(i+1).trimmed();
                this->addToList(channel,url);
            }
        }
    }
}

void ChannelList::saveList()
{
    QString list(QDir::homePath().append("/.config/QSLive/channel"));
    QTreeWidgetItem *item=treeWidget->topLevelItem(0);
    QTreeWidgetItem *child;
    int c=item->childCount();
    QFile f(list);
    if(f.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream out(&f);
        for(int i=0;i<c;i++)
        {
            child=item->child(i);
            QString name=child->text(0);
            QString url=allChannels[name];
            out<<name<<"="<<url<<"\n";
        }
        f.close();
    }
}

void ChannelList::handleView()
{
    QTreeWidgetItem *item=treeWidget->currentItem();
    QString name=item->text(0);
    QString url=allChannels[name];
    QString info(QObject::tr("Channel Name="));
    info.append(name).append("\n");
    info.append(QObject::tr("Channel Url=")).append(url);
    QMessageBox::information(this,QObject::tr("Channel Info"),info);
}

void ChannelList::handleAdd()
{
    QTreeWidgetItem *item=treeWidget->currentItem();
    QString name=item->text(0);
    QString url=allChannels[name];
    addToList(name,url);
    saveList();
}

void ChannelList::handleRemove()
{
    QTreeWidgetItem *item=treeWidget->currentItem();
    QTreeWidgetItem *parentItem=item->parent();
    if(parentItem!=0)
    {
        int index=parentItem->indexOfChild(item);
        QString name=item->text(0);
        parentItem->takeChild(index);
        allChannels.take(name);
        saveList();
    }

}

void ChannelList::handleModifyName()
{
    QTreeWidgetItem *item=treeWidget->currentItem();
    QString name=item->text(0);
    QString url=allChannels[name];
    bool ok;
    QString newName=QInputDialog::getText(this,QObject::tr("Please input the New Channel Name"),QObject::tr("New Channel Name"),QLineEdit::Normal,name,&ok);
    if(ok&&newName!=name&&(!newName.isEmpty()))
    {
        item->setText(0,newName);
        allChannels.take(name);
        allChannels.insert(newName,url);
        saveList();
    }
}

void ChannelList::handleModifyUrl()
{
    QTreeWidgetItem *item=treeWidget->currentItem();
    QString name=item->text(0);
    QString url=allChannels[name];
    bool ok;
    QString newUrl=QInputDialog::getText(this,QObject::tr("Please input the New Channel Url"),QObject::tr("New Channel Url"),QLineEdit::Normal,url,&ok);
    if(ok&&newUrl!=url&&newUrl.startsWith("sop://"))
    {
        allChannels.take(name);
        allChannels.insert(name,newUrl);
        saveList();
    }
}

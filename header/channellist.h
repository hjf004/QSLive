#ifndef CHANNELLIST_H
#define CHANNELLIST_H
#include<QtGui>
#include<QtNetwork>
#include<QtCore/QProcess>
#include"header/xmlparser.h"
#include"header/spprocess.h"
#include"header/treewidget.h"
#include"header/centralwidget.h"
class ChannelList:public QWidget
{
    Q_OBJECT
public:
    ChannelList(QString,QWidget* parent=0);
    QMap<QString,QString> allChannels;
    TreeWidget *treeWidget;
    void setListUrl(QString);
    QString getListUrl();
    QTreeWidgetItem* addToList(QString,QString);
    void readList();
    void saveList();
    bool inTheFavoriteList(QString);
	void setChannelLabel(QString);
private:
    QToolButton *refreshButton;
    QLabel *infoLabel;
    QMap<QString,QStringList> channels;
    XmlParser *parser;
    QUrl listUrl;
    QNetworkReply *rp;
public slots:
    void RefreshList();
    void getList();
    void handleInit();
    void handleView();
    void handleAdd();
    void handleRemove();
    void handleModifyName();
    void handleModifyUrl();
signals:
    void getFinished();
    void playRequest();
    void listReady(bool);
};

#endif // CHANNELLIST_H

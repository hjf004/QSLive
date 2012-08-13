#ifndef SPPROCESS_H
#define SPPROCESS_H
#include<QtCore>
#include"header/utils.h"
class SpProcess:public QProcess
{
    Q_OBJECT
public:
    SpProcess(QObject *parent=0);
    void setBin(QString);
    QString getPlayUrl();
    QString getChannelUrl();
    QString getBin();
    QStringList getPorts();
    bool startSp(QString);
    void setPort(bool,int,int);
    bool customPort;
private:
    QStringList args;
    QString playUrl;
    QString bin;
    QString channelUrl;
    char inport[8],outport[8];
signals:
    void ready(QString,int);
    void progressInfo(int);
    void readyPlay();
public slots:
    void parseState();
    void parseOut();
};

#endif // SPPROCESS_H

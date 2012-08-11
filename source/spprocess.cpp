#include"header/spprocess.h"
#include<cstring>
#include<cstdlib>
SpProcess::SpProcess(QObject *parent):QProcess(parent)
{
    bin.append("sp-sc-auth");
    playUrl="http://localhost:";
    customPort=false;
}

void SpProcess::setBin(QString s)
{
    bin=s;
}

bool SpProcess::startSp(QString url)
{   srand((unsigned int)time(NULL));
    channelUrl=url;
    args.clear();
    if(!customPort)
    {
        int i, j;
        int sock;
        ///choose inport and outport number
        while (1) {
            i = rand () % (65536 - 3000) + 3000;	////3000 to 65535
            sock = connect_to_server ("127.0.0.1", i);
            if (sock == -1)
                break;
            else if (sock >= 0)
                ::close(sock);
        }
        sprintf (inport, "%d", i);
        while (1) {
            j = rand () % (65536 - 3000) + 3000;
            sock = connect_to_server ("127.0.0.1", j);
            if (sock == -1 && j != i)
                break;
            else if (sock >= 0)
                ::close(sock);
        }
        sprintf (outport, "%d", j);
    }
    args<<channelUrl<<QString(inport)<<QString(outport);
    start(bin,args);
    if(waitForStarted())
    {
        return true;
    }
    return false;
}

QString SpProcess::getPlayUrl()
{
    QString p(playUrl);
    p.append(outport).append("/tv.asf");
    return p;
}

QString SpProcess::getChannelUrl()
{
    return channelUrl;
}

void SpProcess::parseOut()
{
    QString out(this->readAll());
    if(out.contains("nblockAvailable"))
    {
        int i=out.lastIndexOf(QString("nblockAvailable="));
        int len=strlen("nblockAvailable=");
        int p=out.mid(i+len,3).toInt();
        if(p>100)p=100;
        emit progressInfo(p);
    }
}

void SpProcess::parseState()
{
    QString out(this->readAll());
    if(out.contains("downloadRate"))
    {
        emit readyPlay();
    }
}

void SpProcess::setPort(bool b, int in, int out)
{
    customPort=b;
    sprintf(inport,"%d",in);
    sprintf(outport,"%d",out);

}

QString SpProcess::getBin()
{
    return bin;
}

QStringList SpProcess::getPorts()
{
    QStringList ports;
    if(customPort)
    {
        QString in(inport);
        QString out(outport);
        ports<<inport<<outport;
    }
    else
        ports<<QString("random")<<QString("random");
    return ports;
}

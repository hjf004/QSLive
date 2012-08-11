#include"header/xmlparser.h"

XmlParser::XmlParser(QByteArray content)
{
    QString errorMessage;
    int errorLine,errorColumn;
    if(!document.setContent(content,false,&errorMessage,&errorLine,&errorColumn))
    {
        qDebug()<<errorMessage<<" Line "<<errorLine<<" Column "<<errorColumn;
    }
    parse();
}

QMap<QString,QStringList> XmlParser::getInfo()
{
    return this->info;
}

bool XmlParser::parse()
{
    QDomElement element=document.documentElement();
    QDomNodeList groups=element.elementsByTagName("group");
    for(int i=0;i<groups.size();i++)
    {
        QStringList cs;
        QString gp;
        QDomElement e=groups.at(i).toElement();
        if(!e.attribute("cn").isEmpty())
            gp=e.attribute("cn");
        else
            gp=e.attribute("en");
        QDomNodeList achannels=e.elementsByTagName("channel");
        for(int j=0;j<achannels.size();j++)
        {
            QDomElement achannel=achannels.at(j).toElement();
            QDomNodeList n=achannel.elementsByTagName("name");
            if(n.isEmpty())
            {
                qDebug()<<"Parse Error!";
                return false;
            }
            QDomElement name=n.at(0).toElement();
            if(!name.attribute("cn").isEmpty())
                cs<<name.attribute("cn");
            else
                cs<<name.attribute("en");
            QDomNodeList addressList=achannel.elementsByTagName("sop_address");
            if(addressList.isEmpty())
            {
                qDebug()<<"Parse Error!";
                return false;
            }
            QDomNodeList address=addressList.at(0).toElement().elementsByTagName("item");
            if(address.isEmpty())
            {
                qDebug()<<"Parse Error!";
                return false;
            }
            QDomElement add=address.at(0).toElement();
            cs<<add.text();
        }
        this->info.insert(gp,cs);
    }
    return true;
}

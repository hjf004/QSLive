#ifndef XMLPARSER_H
#define XMLPARSER_H
#include<QtCore>
#include<QtXml/QDomDocument>
#include<QtXml/QDomElement>
#include<QtXml/QDomAttr>

class XmlParser
{
public:
    XmlParser(QByteArray content);
    QMap<QString,QStringList> getInfo();
private:
    QMap<QString,QStringList> info;
    QDomDocument document;
    bool parse();
};

#endif // XMLPARSER_H

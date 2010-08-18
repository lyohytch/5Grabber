#ifndef CPARSERHANDLER_H
#define CPARSERHANDLER_H

#include <QObject>

#include <cdatastructure.h>
class CParserHandler : public QObject
{
Q_OBJECT
public:
    explicit CParserHandler(QObject *parent = 0);
    bool init() {return true;}
signals:

public slots:
    void startParsing(CDataStructure* data);

};

#endif // CPARSERHANDLER_H

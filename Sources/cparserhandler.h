#ifndef CPARSERHANDLER_H
#define CPARSERHANDLER_H

#include <QObject>

#include "cdatabasehandler.h"
class CParserHandler : public QObject
{
Q_OBJECT
public:
    explicit CParserHandler(QObject *parent = 0);
    bool init() {return true;}
signals:

public slots:
    void startParsing(CDataBaseHandler* database);

};

#endif // CPARSERHANDLER_H

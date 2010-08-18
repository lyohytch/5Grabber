#include "cparserhandler.h"

#include <QDebug>
#include <QString>

CParserHandler::CParserHandler(QObject *parent) :
    QObject(parent)
{
}

void CParserHandler::startParsing(CDataStructure* data)
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"Parsing started";
}

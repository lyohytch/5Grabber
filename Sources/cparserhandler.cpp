#include "cparserhandler.h"

#include <QDebug>
#include <QString>

CParserHandler::CParserHandler(QObject *parent) :
    QObject(parent)
{
}

void CParserHandler::startParsing(CDataBaseHandler* database)
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"Parsing started";
    qDebug()<<database->read().size();
}

#include "cparserhandler.h"
#include "parsedocxls.h"
#include <QDebug>
#include <QString>

CParserHandler::CParserHandler(QObject *parent) :
    QObject(parent)
{
}

void CParserHandler::startParsing(CDataBaseHandler* database)
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"Parsing started";
    QString fname = "parser_analisys.doc";
    ParseDoc pD(this, fname);
    pD.ParseFile(fname);
}

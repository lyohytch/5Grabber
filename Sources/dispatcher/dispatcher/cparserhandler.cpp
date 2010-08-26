#include "cparserhandler.h"

#include <constants.h>
#include <QString>


CParserHandler::CParserHandler(QObject *parent) :
    QObject(parent)
{
    m_parcer = new TParseReceiver();
}

CParserHandler::~CParserHandler()
{
    delete m_parcer;
}

bool CParserHandler::init()
{
    return connect(m_parcer, SIGNAL(finishedParse(int,const QByteArray)), this, SLOT(onParceFinished(int,const QByteArray)),Qt::QueuedConnection);
}

void CParserHandler::startParsing(CDataStructure *data, const QUrl &url)
{
    qDebug()<<"Parsing started";
    m_parcer->parse(data, url.toString().toUtf8());
}

void CParserHandler::onParceFinished(int _error, const QByteArray _url)
{
    qDebug()<<"Parse finished with code:"<<_error<<_url;
    QString urlStr(_url);
    qDebug()<<urlStr;
    QUrl url(urlStr);
    qDebug()<<"Parse finished with code:"<<_error<<url;
    emit parceFinished(_error, url);
}

#include "cparserhandler.h"

#include <QDebug>
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
    return connect(m_parcer, SIGNAL(finished(int,const QByteArray)), this, SLOT(onParceFinished(int,const QByteArray)),Qt::QueuedConnection);
}

void CParserHandler::startParsing(CDataStructure *data, const QUrl &url)
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"Parsing started";
    m_parcer->parse(data, url.toString().toUtf8());
}

void CParserHandler::onParceFinished(int _error, const QByteArray _url)
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"Parse finished with code:"<<_error<<_url;
//    QString urlStr;
//    urlStr.fromUtf8(_url);
//    QUrl url(urlStr);
//    emit parceFinished(_error, url);
}

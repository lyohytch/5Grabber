#include "crecivethread.h"
#include <QDebug>

CReciveThread :: CReciveThread(QUrl url, int id, QObject *parent) : QThread(parent)
        ,m_url(url)
        ,m_threadId(id)
{
}

CReciveThread :: ~CReciveThread()
{
    m_http.close();
}

void CReciveThread :: run()
{
    m_http.setHost(m_url.host());
    connect(&m_http, SIGNAL(requestFinished(int,bool)), this, SLOT(onRecieveComplete(int,bool)));
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<QString("%1?%2").arg(m_url.path()).arg(QString(m_url.encodedQuery()));
    m_httpId=m_http.get(QString("%1?%2").arg(m_url.path()).arg(QString(m_url.encodedQuery())));
    exec();
}

void CReciveThread::onRecieveComplete(int id, bool error)
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;

    if(m_httpId!=id)
    {
        return;
    }

    if(error)
    {
        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"Could not recieve requested URL";
    }

    QByteArray data=m_http.readAll();
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<data.size();
    m_data->write(data);
//    emit dataReady(m_threadId,data);
    emit dataReady(m_threadId);
}


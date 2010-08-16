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
    m_httpId=m_http.get(m_url.path());
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
    emit dataReady(m_threadId,data);
}

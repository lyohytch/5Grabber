#include "crecivethread.h"
#include <QDebug>
#include <QNetworkProxy>

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

    if(m_url.scheme()==QString("http") && QProcessEnvironment::systemEnvironment().value("http_proxy")!=QString(""))
    {
        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"Setup http_proxy="<<QProcessEnvironment::systemEnvironment().value("http_proxy");
        m_http.setProxy(QUrl(QProcessEnvironment::systemEnvironment().value("http_proxy")).host(),QUrl(QProcessEnvironment::systemEnvironment().value("http_proxy")).port());
    }

    if(m_url.scheme()==QString("https") && QProcessEnvironment::systemEnvironment().value("https_proxy")!=QString(""))
    {
        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"Setup https_proxy="<<QProcessEnvironment::systemEnvironment().value("https_proxy");
        m_http.setProxy(QUrl(QProcessEnvironment::systemEnvironment().value("https_proxy")).host(),QUrl(QProcessEnvironment::systemEnvironment().value("https_proxy")).port());
    }

    connect(&m_http, SIGNAL(requestFinished(int,bool)), this, SLOT(onRecieveComplete(int,bool)));
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"dataPointer raw value:"<<m_data<<"threadId:"<<m_threadId<<"path:"<<QString("%1?%2").arg(m_url.path()).arg(QString(m_url.encodedQuery()));
    m_httpId=m_http.get(QString("%1?%2").arg(m_url.path()).arg(QString(m_url.encodedQuery())));
    exec();
}

void CReciveThread::onRecieveComplete(int id, bool error)
{
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
    //qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<data.data();
    m_data->write(data);
//    emit dataReady(m_threadId,data);
    emit dataReady(m_threadId);
}


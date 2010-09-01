#include "crecivethread.h"
#include <cconfighandler.h>
#include <QNetworkProxy>

#include <constants.h>

#ifdef TIME_STAMPS
    unsigned int CReciveThread :: gSummaryDownloadTime=0;
#endif

CReciveThread :: CReciveThread(QUrl url, int id, QObject *parent) : QThread(parent)
        ,m_url(url)
        ,m_threadId(id)
{
#ifdef TIME_STAMPS
    m_TmpDownloadStartTime=QDateTime::currentDateTime().toTime_t();
#endif
    connect(&m_timeout, SIGNAL(timeout()), &m_http, SLOT(abort()));
    m_timeout.setSingleShot(true);
    m_timeout.start((getConfigurationValue("zakazrf_ru/connection_timout", 10).toInt())*1000);
}

CReciveThread :: ~CReciveThread()
{
    m_http.close();
    disconnect(&m_http, SIGNAL(requestFinished(int,bool)), this, SLOT(onRecieveComplete(int,bool)));
}

void CReciveThread :: run()
{
    m_http.setHost(m_url.host());

    if(m_url.scheme()==QString("http") && QProcessEnvironment::systemEnvironment().value("http_proxy")!=QString(""))
    {
        qDebug()<<"Setup http_proxy="<<QProcessEnvironment::systemEnvironment().value("http_proxy");
        m_http.setProxy(QUrl(QProcessEnvironment::systemEnvironment().value("http_proxy")).host(),QUrl(QProcessEnvironment::systemEnvironment().value("http_proxy")).port());
    }

    if(m_url.scheme()==QString("https") && QProcessEnvironment::systemEnvironment().value("https_proxy")!=QString(""))
    {
        qDebug()<<"Setup https_proxy="<<QProcessEnvironment::systemEnvironment().value("https_proxy");
        m_http.setProxy(QUrl(QProcessEnvironment::systemEnvironment().value("https_proxy")).host(),QUrl(QProcessEnvironment::systemEnvironment().value("https_proxy")).port());
    }

    connect(&m_http, SIGNAL(requestFinished(int,bool)), this, SLOT(onRecieveComplete(int,bool)));
    qDebug()<<"dataPointer raw value:"<<m_data<<"threadId:"<<m_threadId<<"path:"<<QString("%1?%2").arg(m_url.path()).arg(QString(m_url.encodedQuery()));
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
        qDebug()<<"Could not recieve requested URL ("<<m_http.error()<<")"<<m_http.errorString();
        if(m_data->needRequeue()>0)
        {
            m_data->setNeedRequeue(m_data->needRequeue()-1);
        }
        else
        {
            m_data->setNeedRequeue(getConfigurationValue("zakazrf_ru/attempts", 10).toInt());
        }
        qDebug()<<"Attempts amount :"<<m_data->needRequeue();
        emit dataReady(m_threadId);
        return;
    }

    QByteArray data=m_http.readAll();
    qDebug()<<data.size();
    m_data->write(data);
#ifdef TIME_STAMPS
    gSummaryDownloadTime+=QDateTime::currentDateTime().toTime_t()-m_TmpDownloadStartTime;
    qDebug()<<"######################## Summary download time"<<gSummaryDownloadTime<<" ##########################";
#endif
    m_data->setNeedRequeue(0);
    emit dataReady(m_threadId);
}

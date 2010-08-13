#include "crecievetask.h"
#include "cconfighandler.h"
#include <QDebug>

CRecieveTask::CRecieveTask(CDataBaseHandler* pDataBase, const siterules_ti& rule) : m_pDataBase(pDataBase)
{
    m_url=rule.key();
    m_rules=rule.value();
}

bool CRecieveTask::run()
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
    for(int i=0; m_rules.count()>0 && i<(g_config.valueOf("core_max_threads").toInt()); i++)
    {
        CReciveThread *thread=new CReciveThread(createFullUrlFromRule(m_url, m_rules.takeFirst()), i);
        m_threads.push_back(thread);
        connect(thread, SIGNAL(dataReady(int,QByteArray)), this, SLOT(onDataReady(int,QByteArray)));
        connect(thread, SIGNAL(finished()), this, SLOT(onThreadFinished()));
        thread->start();
    }
    if(m_rules.count()>0)
    {
        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"Thread pool is full. New Threads will be oppened afer finished one of runned";
    }
    return true;
}

void  CRecieveTask::onThreadFinished()
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
    QMutex m_mutex;
    m_mutex.lock();
    for(int i=0; i<m_threads.count(); i++)
    {
        if(!m_threads.at(i)->isFinished())
        {
            continue;
        }

        delete m_threads.at(i);
        m_threads.removeAt(i);
        for(int i=0; m_rules.count()>0 && i<(g_config.valueOf("core_max_threads").toInt()); i++)
        {
            qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"Run not finnished task";
            CReciveThread *thread=new CReciveThread(createFullUrlFromRule(m_url, m_rules.takeFirst()), i);
            m_threads.push_back(thread);
            connect(thread, SIGNAL(dataReady(int,QByteArray)), this, SLOT(onDataReady(int,QByteArray)));
            connect(thread, SIGNAL(finished()), this, SLOT(onThreadFinished()));
            thread->start();
        }
    }

    if(m_threads.count()<=0 && m_rules.count()<=0)
    {
        emit finished(this);
    }

    m_mutex.unlock();
}

void CRecieveTask::destroy()
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
}


void CRecieveTask::onDataReady(int i, QByteArray data)
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<i;
    m_pDataBase->write(data);
    m_threads.at(i)->exit(0);
}


QUrl CRecieveTask::createFullUrlFromRule(QUrl url, QVariant rule)
{
    url.setPath(rule.toString());
    return url;
}





CReciveThread :: CReciveThread(QUrl url, int id, QObject *parent) :
        m_threadId(id)
        ,m_url(url)
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
//    exit(0);
}

#include "crecievetask-zakazrf_ru.h"
#include <QDebug>

CRecieveTask_zakazrf_ru::CRecieveTask_zakazrf_ru()
{
    m_signaller=new CRecieveTaskSignaller;
}

bool CRecieveTask_zakazrf_ru::init(int maxThreads, const siterules_ti &rule)
{
    m_url=rule.key();
    m_rules=rule.value();
    m_maxThreads=maxThreads;
    return true;
}

CRecieveTaskSignaller* CRecieveTask_zakazrf_ru::signaller()
{
    return m_signaller;
}

bool CRecieveTask_zakazrf_ru::run()
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
    for(int i=0; m_rules.count()>0 && i<(m_maxThreads); i++)
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

void  CRecieveTask_zakazrf_ru::onThreadFinished()
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
        for(int i=0; m_rules.count()>0 && i<(m_maxThreads); i++)
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
        m_signaller->onRecieveFinished(this);
    }

    m_mutex.unlock();
}

void CRecieveTask_zakazrf_ru::onDataReady(int i, QByteArray data)
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<i;
//    m_pDataBase->write(data);
    m_threads.at(i)->exit(0);
}


QUrl CRecieveTask_zakazrf_ru::createFullUrlFromRule(QUrl url, QVariant rule)
{
    url.setPath(rule.toString());
    return url;
}

Q_EXPORT_PLUGIN2(recievetask_4_zakazrf_ru, CRecieveTask_zakazrf_ru)

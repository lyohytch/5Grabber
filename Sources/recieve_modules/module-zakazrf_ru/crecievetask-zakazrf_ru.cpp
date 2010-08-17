#include "crecievetask-zakazrf_ru.h"
#include <QDebug>
#include <QFile>

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

    QUrl testUrl("http://zakazrf.ru/ViewReduction.aspx?id=2943");
    CDataStructure* data = new CDataStructure(testUrl);
    m_dataStructures.push_back(data);
    data->setType(getUrlDataType(testUrl));
    data->setRoot();

    for(int i=0; i<m_dataStructures.count() && i<(m_maxThreads); i++)
    {
        if(m_dataStructures.at(i)->isDone())
        {
            continue;
        }

        CReciveThread *thread=new CReciveThread(m_dataStructures.at(i)->url(), i);
        thread->setDataStructure(m_dataStructures.value(i));
        m_threads.push_back(thread);
        connect(thread, SIGNAL(dataReady(int/*,QByteArray*/)), this, SLOT(onDataReady(int/*,QByteArray*/)));
        connect(thread, SIGNAL(finished()), this, SLOT(onThreadFinished()));
        thread->start();
    }
//    for(int i=0; m_rules.count()>0 && i<(m_maxThreads); i++)
//    {
//        CReciveThread *thread=new CReciveThread(createFullUrlFromRule(m_url, m_rules.takeFirst()), i);
//        thread->setDataStructure(data);
//        m_threads.push_back(thread);
//        connect(thread, SIGNAL(dataReady(int,QByteArray)), this, SLOT(onDataReady(int,QByteArray)));
//        connect(thread, SIGNAL(finished()), this, SLOT(onThreadFinished()));
//        thread->start();
//    }
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
//    for(int i=0; i<m_threads.count(); i++)
//    {
//        if(!m_threads.at(i)->isFinished())
//        {
//            continue;
//        }
//
//        delete m_threads.at(i);
//        m_threads.removeAt(i);
//        for(int i=0; m_rules.count()>0 && i<(m_maxThreads); i++)
//        {
//            qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"Run not finnished task";
//            CReciveThread *thread=new CReciveThread(createFullUrlFromRule(m_url, m_rules.takeFirst()), i);
//            m_threads.push_back(thread);
//            connect(thread, SIGNAL(dataReady(int,QByteArray)), this, SLOT(onDataReady(int,QByteArray)));
//            connect(thread, SIGNAL(finished()), this, SLOT(onThreadFinished()));
//            thread->start();
//        }
//    }
//
//    if(m_threads.count()<=0 && m_rules.count()<=0)
//    {
//        m_signaller->onRecieveFinished(this);
//    }

    for(int i=0; i<m_threads.count(); i++)
    {
        if(!m_threads.at(i)->isFinished())
        {
            continue;
        }

        delete m_threads.at(i);
        m_threads.removeAt(i);
    }

    for(int i=0; i<m_dataStructures.count() && i<(m_maxThreads); i++)
    {
        if(m_dataStructures.at(i)->isDone())
        {
            continue;
        }

        if(m_dataStructures.at(i)->isRoot() && m_dataStructures.at(i)->isFinished())
        {
            m_signaller->onDataReady(m_dataStructures.value(i));
            continue;
        }

        CReciveThread *thread=new CReciveThread(m_dataStructures.at(i)->url(), i);
        thread->setDataStructure(m_dataStructures.value(i));
        m_threads.push_back(thread);
        connect(thread, SIGNAL(dataReady(int/*,QByteArray*/)), this, SLOT(onDataReady(int/*,QByteArray*/)));
        connect(thread, SIGNAL(finished()), this, SLOT(onThreadFinished()));
        thread->start();
    }

    if(m_dataStructures.count()<=0 && m_threads.count()<=0)
    {
        m_signaller->onRecieveFinished(this);
    }
    m_mutex.unlock();
}

void CRecieveTask_zakazrf_ru::onDataReady(int threadId/*, QByteArray data*/)
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<threadId;
    //qDebug()<<m_threads.at(threadId)->data()->read();
    CDataStructure* data=m_threads.at(threadId)->data();

    QList<QRegExp> regexps;
    regexps.push_back(QRegExp("ViewLot.aspx\\?id=[0-9]{1,}", Qt::CaseSensitive));
//    regexps.push_back(QRegExp("DFile.ashx\\?id=[0-9]{1,}", Qt::CaseSensitive));

    if(data->type()==CDataStructure::eDataTypeDocument)
    {
        data->done();
        m_threads.at(threadId)->exit(0);
        return;
    }

    QStringList childLinks=data->findLinks(regexps);
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"Start processing child links: "<<childLinks;
    CDataStructure* child;
    for(int i=0; i<childLinks.count(); i++)
    {
        QUrl newUrl=QUrl(QString("%1://%2/%3").arg(data->url().scheme()).arg(data->url().host()).arg(childLinks.at(i)));
        if(newUrl==data->url())
        {
            //It's hack for now
            continue;
        }
        child = new CDataStructure(newUrl);
        child->setType(getUrlDataType(newUrl));
        data->appendChild(child);
        m_dataStructures.push_back(child);
    }

    data->done();
    m_threads.at(threadId)->exit(0);
}


QUrl CRecieveTask_zakazrf_ru::createFullUrlFromRule(QUrl url, QVariant rule)
{
    url.setPath(rule.toString());
    return url;
}

int CRecieveTask_zakazrf_ru::getUrlDataType(QUrl &url)
{
    if(url.toString().contains("DFile.ashx"))
    {
        return CDataStructure::eDataTypeDocument;
    }

    return CDataStructure::eDataTypePage;
}

Q_EXPORT_PLUGIN2(recievetask_4_zakazrf_ru, CRecieveTask_zakazrf_ru)

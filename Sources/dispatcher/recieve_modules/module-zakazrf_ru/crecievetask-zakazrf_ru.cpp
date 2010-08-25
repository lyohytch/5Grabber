#include "crecievetask-zakazrf_ru.h"
#include <QDebug>
#include <QFile>
#include <QThreadPool>
#include <QDateTime>

#define RUN_ALL_TASKS

CRecieveTask_zakazrf_ru::CRecieveTask_zakazrf_ru()
{
    m_threadCounter=0;
    m_signaller=new CRecieveTaskSignaller;
}

CRecieveTask_zakazrf_ru::~CRecieveTask_zakazrf_ru()
{
    delete m_signaller;

    for(int i=0; i<m_threads.count(); i++)
    {
        delete m_threads.value(i);
    }
    m_threads.clear();

    m_activeDataStructures.clear();

    QMultiMap<QUrl, CDataStructure*>::iterator dataStructuresIter;
    for(dataStructuresIter=m_dataStructures.begin(); dataStructuresIter!=m_dataStructures.end(); dataStructuresIter++)
    {
       delete dataStructuresIter.value();
    }
    m_dataStructures.clear();
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

QString CRecieveTask_zakazrf_ru::taskHost() const
{
    return QString("zakazrf.ru");
}


bool CRecieveTask_zakazrf_ru::run()
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;

    connect(m_signaller, SIGNAL(dataParsed(QUrl)), this, SLOT(removeData(QUrl)));
#ifndef RUN_ALL_TASKS
    QUrl testUrl("http://zakazrf.ru/ViewReduction.aspx?id=4781");
    CDataStructure* tmpdata = new CDataStructure(testUrl);
    tmpdata->setType(getUrlDataType(testUrl));
    tmpdata->setRoot();
    m_dataStructures.insert(testUrl, tmpdata);
    m_activeDataStructures.push_back(tmpdata);
    QUrl testUrl2("http://zakazrf.ru/ViewReduction.aspx?id=2943");
    CDataStructure* tmpdata2 = new CDataStructure(testUrl2);
    tmpdata2->setType(getUrlDataType(testUrl2));
    tmpdata2->setRoot();
    m_dataStructures.insert(testUrl2, tmpdata2);
    m_activeDataStructures.push_back(tmpdata2);
    QUrl testUrl3("http://zakazrf.ru/ViewReduction.aspx?id=5319");
    CDataStructure* tmpdata3 = new CDataStructure(testUrl3);
    tmpdata3->setType(getUrlDataType(testUrl3));
    tmpdata3->setRoot();
    m_dataStructures.insert(testUrl3, tmpdata3);
    m_activeDataStructures.push_back(tmpdata3);
#else
for(int i=2000; i<10000; i++)
{
    QUrl testUrl(QString("http://zakazrf.ru/ViewReduction.aspx?id=%1").arg(i));
    CDataStructure* tmpdata = new CDataStructure(testUrl);
    tmpdata->setType(getUrlDataType(testUrl));
    tmpdata->setRoot();
    m_dataStructures.insert(testUrl, tmpdata);
    m_activeDataStructures.push_back(tmpdata);
}
#endif

    CDataStructure* data=NULL;
    QList<CDataStructure*>::iterator activeDataStructuresIter;
    for(activeDataStructuresIter = m_activeDataStructures.begin(); activeDataStructuresIter != m_activeDataStructures.end();)
    {
        if(m_threads.count()<(m_maxThreads+1))
        {
            data=(*activeDataStructuresIter);
            CReciveThread *thread=new CReciveThread(data->url(), m_threadCounter++);
            thread->setDataStructure(data);
            m_threads.push_back(thread);
            connect(thread, SIGNAL(dataReady(int/*,QByteArray*/)), this, SLOT(onDataReady(int/*,QByteArray*/)));
            connect(thread, SIGNAL(finished()), this, SLOT(onThreadFinished()));
            thread->start();
            activeDataStructuresIter=m_activeDataStructures.erase(activeDataStructuresIter);
            break;
        }
        break;
    }

//    if(m_rules.count()>0)
//    {
//        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"Thread pool is full. New Threads will be oppened afer finished one of runned";
//    }
    return true;
}

void  CRecieveTask_zakazrf_ru::onThreadFinished()
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
    QMutex m_mutex;
    m_mutex.lock();

    QList<CReciveThread*>::iterator threadIter;
    for(threadIter = m_threads.begin(); threadIter != m_threads.end();)
    {
        CReciveThread* thread=(*threadIter);
        if(!thread)
        {
            qCritical()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"Found NULL pointer to thread\n remove it";
            threadIter=m_threads.erase(threadIter);
            continue;
        }

        if(thread->isFinished())
        {
            delete thread;
            threadIter=m_threads.erase(threadIter);
            continue;
        }
        threadIter++;
    }

    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"Active data structures amount:"<<m_activeDataStructures.count();

    QList<CDataStructure*>::iterator activeDataStructuresIter;
    for(activeDataStructuresIter = m_activeDataStructures.begin(); activeDataStructuresIter != m_activeDataStructures.end();)
    {
        CDataStructure* data=(*activeDataStructuresIter);
        if(!data)
        {
            qCritical()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"Found NULL pointer to data structure in active structures\n remove it";
            activeDataStructuresIter=m_activeDataStructures.erase(activeDataStructuresIter);
            continue;
        }

        if(data->isRoot() && data->isFinished())
        {
            qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"Found finished data structure in task list\n remove it";
            activeDataStructuresIter=m_activeDataStructures.erase(activeDataStructuresIter);
            continue;
        }

        if(data->isDone())
        {
            qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"Found done data structure in task list\n remove it";
            activeDataStructuresIter=m_activeDataStructures.erase(activeDataStructuresIter);
            continue;
        }

        if(m_threads.count()<(m_maxThreads+1))
        {
            CReciveThread *thread=new CReciveThread(data->url(),m_threadCounter++);
            thread->setDataStructure(data);
            m_threads.push_back(thread);
            connect(thread, SIGNAL(dataReady(int/*,QByteArray*/)), this, SLOT(onDataReady(int/*,QByteArray*/)));
            connect(thread, SIGNAL(finished()), this, SLOT(onThreadFinished()));
            thread->start();
            activeDataStructuresIter=m_activeDataStructures.erase(activeDataStructuresIter);
            continue;
        }
        break;
    }
    m_mutex.unlock();
}

void CRecieveTask_zakazrf_ru::onDataReady(int threadId/*, QByteArray data*/)
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<threadId;

    QList<QRegExp> regexps;
    regexps.push_back(QRegExp("ViewLot.aspx\\?id=[0-9]{1,}", Qt::CaseSensitive));
    regexps.push_back(QRegExp("DFile.ashx\\?id=[0-9]{1,}", Qt::CaseSensitive));
    regexps.push_back(QRegExp("ViewLotStatisticPre.aspx\\?id=[0-9]{1,}", Qt::CaseSensitive));

    CDataStructure* data=NULL;
    int threadNum=-1;
    for(int i=0; i<m_threads.count(); i++)
    {
        if(m_threads.at(i)->id()==threadId)
        {
            threadNum=i;
            data=m_threads.at(i)->data();
            break;
        }
    }

    if(!data)
    {
        qCritical()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"FATAL ERROR"<<"Thread with id:"<<threadId<<"not found";
        m_threads.at(threadNum)->exit(0);
        return;
    }

    if(data->type()==CDataStructure::eDataTypeDocument)
    {
        data->done();
        if(data->root()->isFinished())
        {
            m_signaller->onDataReady(data->root());
        }
        m_threads.at(threadNum)->exit(0);
        return;
    }

    QStringList childLinks=findLinks(regexps, data->read());
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"Start processing child links: "<<childLinks;

    CDataStructure* child;
    for(int i=0; i<childLinks.count(); i++)
    {
        QUrl newUrl=QUrl(QString("%1://%2/%3").arg(data->url().scheme()).arg(data->url().host()).arg(childLinks.at(i)));
        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"Full child url:"<<newUrl;
        if(data->root()->contains(newUrl))
        {
            qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"Url already in structure: "<<newUrl;
            continue;
        }

        child = new CDataStructure(newUrl);
        child->setRoot(data->root());
        child->setType(getUrlDataType(newUrl));
        data->appendChild(child);        
        m_dataStructures.insert(data->root()->url(), child);
        m_activeDataStructures.push_front(child);
    }
    data->done();
    if(data->root()->isFinished())
    {
        m_signaller->onDataReady(data->root());
    }
    m_threads.at(threadNum)->exit(0);
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

    if(url.toString().contains("ViewLot.aspx"))
    {
        return CDataStructure::eDataTypeLotPage;
    }

    if(url.toString().contains("ViewLotStatisticPre.aspx"))
    {
        return CDataStructure::eDataTypeLotStatisticPage;
    }

    return CDataStructure::eDataTypeAuctionPage;
}

void CRecieveTask_zakazrf_ru::removeData(QUrl root)
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<root;

    CDataStructure* rootData=NULL;

    QMultiMap<QUrl, CDataStructure*>::iterator dataStructuresIter;
    dataStructuresIter = m_dataStructures.find(root);
    rootData=dataStructuresIter.value()->root();
    rootData->flush();
    m_dataStructures.remove(root);
//    for(dataStructuresIter = m_dataStructures.begin(); dataStructuresIter != m_dataStructures.end();)
//    {
//        CDataStructure* data=dataStructuresIter.value();
//        if(!data)
//        {
//            qCritical()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"Found NULL pointer to data structure in active structures\n remove it";
//            dataStructuresIter=m_dataStructures.erase(dataStructuresIter);
//            continue;
//        }

//        if(data->isRoot())
//        {
//            qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"We should remove root at last";
//            rootData=data;
//            dataStructuresIter++;
//            continue;
//        }

//        if(data->root()->url()==root && !data->isRoot())
//        {
//            delete data;
//            dataStructuresIter=m_dataStructures.erase(dataStructuresIter);
//            continue;
//        }
//        dataStructuresIter++;
//    }

//    m_dataStructures.removeOne(rootData);
//    delete rootData;

    if(m_dataStructures.count()<=0)
    {
        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"Amazing! We just finished task!" << QDateTime::currentDateTime();
        m_signaller->onRecieveFinished(this);
    }
}

QStringList CRecieveTask_zakazrf_ru::findLinks(QList<QRegExp> &regexps, const QByteArray &data)
{
    if(data.isEmpty())
    {
        return QStringList();
    }
    QStringList foundLinks;

    QString str(data);
    for(int i=0;i<regexps.count();i++)
    {
        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;

        QRegExp regexp=regexps.value(i);
        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
        for(int pos=regexp.indexIn(str); pos!=-1; pos=regexp.indexIn(str,pos+1))
        {
            qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
            if(foundLinks.contains(regexp.capturedTexts().at(0)))
            {
                qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
                continue;
            }

            qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
            foundLinks.append(regexp.capturedTexts().at(0));
            qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
        }
        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
    }

    return foundLinks;
}

Q_EXPORT_PLUGIN2(recievetask_4_zakazrf_ru, CRecieveTask_zakazrf_ru)

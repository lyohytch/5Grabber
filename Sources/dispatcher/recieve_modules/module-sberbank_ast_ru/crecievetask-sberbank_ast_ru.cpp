#include "crecievetask-sberbank_ast_ru.h"

#include <QFile>
#include <QThreadPool>
#include <QDateTime>

#include <constants.h>

#define RUN_ALL_TASKS

CRecieveTask_sberbank_ast_ru::CRecieveTask_sberbank_ast_ru()
{
    m_threadCounter=0;
    m_signaller=new CRecieveTaskSignaller;
}

CRecieveTask_sberbank_ast_ru::~CRecieveTask_sberbank_ast_ru()
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

bool CRecieveTask_sberbank_ast_ru::init(int maxThreads, const siterules_ti &rule)
{
    qDebug()<<"Max threads:"<<maxThreads;
    m_url=rule.key();
    m_rules=rule.value();
    m_maxThreads=maxThreads;
    return true;
}

CRecieveTaskSignaller* CRecieveTask_sberbank_ast_ru::signaller()
{
    return m_signaller;
}

QString CRecieveTask_sberbank_ast_ru::taskHost() const
{
    return QString("sberbank-ast.ru");
}


bool CRecieveTask_sberbank_ast_ru::run()
{
    qDebug();

    connect(m_signaller, SIGNAL(dataParsed(QUrl)), this, SLOT(removeData(QUrl)));
#ifndef RUN_ALL_TASKS
    QUrl testUrl("http://sberbank-ast.ru/purchaseview.aspx?id=31223");
    CDataStructure* tmpdata = new CDataStructure(testUrl);
    tmpdata->setType(getUrlDataType(testUrl));
    tmpdata->setRoot();
    m_dataStructures.insert(testUrl, tmpdata);
    m_activeDataStructures.push_back(tmpdata);
    QUrl testUrl2("http://sberbank-ast.ru/purchaseview.aspx?id=31218");
    CDataStructure* tmpdata2 = new CDataStructure(testUrl2);
    tmpdata2->setType(getUrlDataType(testUrl2));
    tmpdata2->setRoot();
    m_dataStructures.insert(testUrl2, tmpdata2);
    m_activeDataStructures.push_back(tmpdata2);
    QUrl testUrl3("http://sberbank-ast.ru/purchaseview.aspx?id=31215");
    CDataStructure* tmpdata3 = new CDataStructure(testUrl3);
    tmpdata3->setType(getUrlDataType(testUrl3));
    tmpdata3->setRoot();
    m_dataStructures.insert(testUrl3, tmpdata3);
    m_activeDataStructures.push_back(tmpdata3);
#else
for(int i=38000; i<100; i--)
{
    QUrl testUrl(QString("http://sberbank-ast.ru/purchaseview.aspx?id=%1").arg(i));
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
//        qDebug()<<"Thread pool is full. New Threads will be oppened afer finished one of runned";
//    }
    return true;
}

void  CRecieveTask_sberbank_ast_ru::onThreadFinished()
{
    qDebug();
    QMutex m_mutex;
    m_mutex.lock();

    QList<CReciveThread*>::iterator threadIter;
    for(threadIter = m_threads.begin(); threadIter != m_threads.end();)
    {
        CReciveThread* thread=(*threadIter);
        if(!thread)
        {
            qCritical()<<"Found NULL pointer to thread\n remove it";
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

    qDebug()<<"Active data structures amount:"<<m_activeDataStructures.count();

    QList<CDataStructure*>::iterator activeDataStructuresIter;
    for(activeDataStructuresIter = m_activeDataStructures.begin(); activeDataStructuresIter != m_activeDataStructures.end();)
    {
        CDataStructure* data=(*activeDataStructuresIter);
        if(!data)
        {
            qCritical()<<"Found NULL pointer to data structure in active structures\n remove it";
            activeDataStructuresIter=m_activeDataStructures.erase(activeDataStructuresIter);
            continue;
        }

        if(data->isRoot() && data->isFinished())
        {
            qDebug()<<"Found finished data structure in task list\n remove it";
            activeDataStructuresIter=m_activeDataStructures.erase(activeDataStructuresIter);
            continue;
        }

        if(data->isDone())
        {
            qDebug()<<"Found done data structure in task list\n remove it";
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

void CRecieveTask_sberbank_ast_ru::onDataReady(int threadId/*, QByteArray data*/)
{
    qDebug()<<"Thread Id: "<<threadId;

    QList<QRegExp> regexps;
    regexps.push_back(QRegExp("docid\\&gt\\;[0-9]{1,}", Qt::CaseSensitive));
//    regexps.push_back(QRegExp("DFile.ashx\\?id=[0-9]{1,}", Qt::CaseSensitive));

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
        qCritical()<<"Thread with id:"<<threadId<<"not found";
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
    qDebug()<<"Start processing child links: "<<childLinks;

    CDataStructure* child;
    for(int i=0; i<childLinks.count(); i++)
    {
        QUrl newUrl=QUrl(QString("%1://%2/ViewDocument.aspx?id=%3").arg(data->url().scheme()).arg(data->url().host()).arg(childLinks.at(i)));
        qDebug()<<"Full child url:"<<newUrl;
        if(data->root()->contains(newUrl))
        {
            qDebug()<<"Url already in structure: "<<newUrl;
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
    regexps.clear();
    m_threads.at(threadNum)->exit(0);
}


QUrl CRecieveTask_sberbank_ast_ru::createFullUrlFromRule(QUrl url, QVariant rule)
{
    url.setPath(rule.toString());
    return url;
}

int CRecieveTask_sberbank_ast_ru::getUrlDataType(QUrl &url)
{
    if(url.toString().contains("ViewDocument.aspx"))
    {
        return CDataStructure::eDataEventPage;
    }

    return CDataStructure::eDataTypeAuctionPage;
}

void CRecieveTask_sberbank_ast_ru::removeData(QUrl root)
{
    qDebug()<<root;

    CDataStructure* rootData=NULL;

    QMultiMap<QUrl, CDataStructure*>::iterator dataStructuresIter;
    dataStructuresIter = m_dataStructures.find(root);
    rootData=dataStructuresIter.value()->root();
    rootData->flush();
    m_dataStructures.remove(root);

    if(m_dataStructures.count()<=0)
    {
        qDebug()<<"Amazing! We just finished task!" << QDateTime::currentDateTime().toTime_t();
        m_signaller->onRecieveFinished(this);
    }
}

QStringList CRecieveTask_sberbank_ast_ru::findLinks(QList<QRegExp> &regexps, const QByteArray &data)
{
    if(data.isEmpty())
    {
        return QStringList();
    }
    QStringList foundLinks;

    QString str(data);
    for(int i=0;i<regexps.count();i++)
    {
        QRegExp regexp=regexps.value(i);
        for(int pos=regexp.indexIn(str); pos!=-1; pos=regexp.indexIn(str,pos+1))
        {
            QString tmp1=regexp.capturedTexts().at(0);
            tmp1.remove(QRegExp("[a-zA-Z;&]"));
            qDebug()<<tmp1;
            if(foundLinks.contains(tmp1))
            {
                qDebug();
                continue;
            }

            foundLinks.append(tmp1);
        }
    }

    return foundLinks;
}
Q_EXPORT_PLUGIN2(recievetask_4_sberbank_ast_ru, CRecieveTask_sberbank_ast_ru)

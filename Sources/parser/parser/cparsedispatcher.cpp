#include "cparsedispatcher.h"
#include "constants.h"

#include <QDebug>
#include <QDateTime>

CParseDispatcher::CParseDispatcher(): m_maxThreads(1)
{
    m_db = new DBmanager();
}

CParseDispatcher::~CParseDispatcher()
{
    if (m_db)
    {
        delete m_db;
    }
}

bool CParseDispatcher::init()
{
    return m_db->init();
}

void CParseDispatcher::addToQueue(CDataStructure *newTask)
{
    m_queue.push_back(newTask);
    onAddedToQueue();
}

void CParseDispatcher::onAddedToQueue()
{
    qDebug();
    if (m_activeQueue.count() < m_maxThreads)
    {
        qDebug() << "QUEUE count = " << m_queue.count();
        if (!m_queue.isEmpty())
        {
            CDataStructure* queueMember = m_queue.takeFirst();
            m_activeQueue.push_back(queueMember);

            // run thread
            CParseThread* thread = new CParseThread(queueMember, m_db);
            connect(thread, SIGNAL(finished()), this, SLOT(onParseTaskFinished()));
            m_threads.insert(thread, queueMember);
//            qDebug() << counter++ << "(((((((((((((((((" << QDateTime::currentDateTime().toTime_t();
            thread->start();
        }
    }
}

void CParseDispatcher::onParseTaskFinished()
{
//    qDebug() << "THREAD FINISHED";
//    qDebug() << "))))))))))))))" << QDateTime::currentDateTime().toTime_t();
    foreach (CParseThread* thread, m_threads.keys())
    {
        qDebug() << "THREADS COUNT BEFORE = " << m_threads.count();
        if (thread->isFinished())
        {
            CDataStructure* parsed_data = m_threads.value(thread);
            m_activeQueue.removeOne(parsed_data);
            m_threads.remove(thread);
            delete thread;
            emit queueMemeberParsed(parsed_data);
        }
    }
    qDebug() << "THREADS COUNT AFTER  = " << m_threads.count();
    while (m_activeQueue.count() < m_maxThreads)
    {
        if (!m_queue.isEmpty())
        {
            CDataStructure* queueMember = m_queue.takeFirst();
            m_activeQueue.push_back(queueMember);

            // run thread
            CParseThread* newThread = new CParseThread(queueMember, m_db);
            connect(newThread, SIGNAL(finished()), this, SLOT(onParseTaskFinished()));
            m_threads.insert(newThread, queueMember);
            newThread->start();
        }
        else
        {
            break;
        }
    }
    qDebug() << "THREADS COUNT AFTER2 = " << m_threads.count();
}

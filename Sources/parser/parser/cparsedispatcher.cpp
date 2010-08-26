#include "cparsedispatcher.h"
#include "constants.h"

#include <QDebug>
#include <QDateTime>

CParseDispatcher::CParseDispatcher(): m_maxThreads(1)
{
}

CParseDispatcher::~CParseDispatcher()
{
    // Nothing
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
            CParseThread* thread = new CParseThread(queueMember);
            connect(thread, SIGNAL(finished()), this, SLOT(onParseTaskFinished()));
            m_threads.insert(thread, queueMember);
            qDebug() << counter++ << "(((((((((((((((((" << QDateTime::currentDateTime().toTime_t();
            thread->start();
        }
    }
}

void CParseDispatcher::onParseTaskFinished()
{
    qDebug() << "THREAD FINISHED";
    qDebug() << "))))))))))))))" << QDateTime::currentDateTime().toTime_t();
    foreach (CParseThread* thread, m_threads.keys())
    {
        if (thread->isFinished())
        {
            CDataStructure* parsed_data = m_threads.value(thread);
            m_activeQueue.removeOne(parsed_data);
            m_threads.remove(thread);
            delete thread;
            emit queueMemeberParsed(parsed_data);
        }

        if (m_activeQueue.count() < m_maxThreads)
        {
            if (!m_queue.isEmpty())
            {
                CDataStructure* queueMember = m_queue.takeFirst();
                m_activeQueue.push_back(queueMember);

                // run thread
                CParseThread* newThread = new CParseThread(queueMember);
                m_threads.insert(newThread, queueMember);
                newThread->start();
            }
        }
    }
}

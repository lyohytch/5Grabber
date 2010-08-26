#include "cparsedispatcher.h"

#include <QDebug>
#include <QMutex>

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
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
    if (m_activeQueue.count() < m_maxThreads)
    {
        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO << "QUEUE count = " << m_queue.count();
        if (!m_queue.isEmpty())
        {
            CDataStructure* queueMember = m_queue.takeFirst();
            m_activeQueue.push_back(queueMember);

            // run thread
            CParseThread* thread = new CParseThread();
            connect(thread, SIGNAL(finished()), this, SLOT(onParseTaskFinished()));
            m_threads.insert(thread, queueMember);
            thread->start();
        }
    }
}

void CParseDispatcher::onParseTaskFinished()
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO << "THREAD FINISHED";
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
                CParseThread* newThread = new CParseThread();
                m_threads.insert(newThread, queueMember);
                newThread->start();
            }
        }
    }
}

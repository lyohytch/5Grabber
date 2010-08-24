#include "cparsedispatcher.h"

#include <QDebug>

CParseDispatcher::CParseDispatcher(): m_maxThreads(1)
{
}

CParseDispatcher::~CParseDispatcher()
{
    // Nothing
}

void CParseDispatcher::onAddedToQueue()
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
    while (m_activeQueue.count() < m_maxThreads)
    {
        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO << "QUEUE count = " << m_queue.count();
        if (!m_queue.isEmpty())
        {
            CDataStructure* queueMember = m_queue.takeFirst();
            m_activeQueue.push_back(queueMember);
        }
        // run thread
    }
}

void CParseDispatcher::onParseTaskFinished(CDataStructure* p_data)
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
    m_activeQueue.removeOne(p_data);
    while (m_activeQueue.count() < m_maxThreads)
    {
        CDataStructure* queueMember = m_queue.takeFirst();
        m_activeQueue.push_back(queueMember);
        // run thread
    }
}

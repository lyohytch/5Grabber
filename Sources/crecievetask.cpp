#include "crecievetask.h"
#include <QDebug>

CRecieveTask::CRecieveTask(CDataBaseHandler* pDataBase, const siterules_ti& rule) : m_pDataBase(pDataBase)
{
    m_url=rule.key();
    m_rules=rule.value();
}

bool CRecieveTask::run()
{
    m_http.setHost(m_url.host());
    connect(&m_http, SIGNAL(requestFinished(int,bool)), this, SLOT(onRecieveComplete(int,bool)));
    m_id=m_http.get(m_url.path());
    return true;
}

void CRecieveTask::onRecieveComplete(int id, bool error)
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;

    if(m_id!=id)
    {
        return;
    }

    if(error)
    {
        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"Could not recieve requested URL";
    }

    QByteArray data=m_http.readAll();
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<data.size();
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"Written to databse"<<m_pDataBase->write(data);
    emit finished(this);
}

void CRecieveTask::destroy()
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
    m_http.close();
}

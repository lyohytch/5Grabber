#include "cdatastructure.h"
#include <QDebug>

CDataStructure::CDataStructure(QUrl url, QObject *parent) :
    QObject(parent)
    ,m_isDone(false)
    ,m_isRoot(false)
//    ,m_isRunned(false)
{
    m_url=url;
}

CDataStructure::~CDataStructure()
{
    flush();
    m_childs.clear();
}

CDataStructure* CDataStructure::childAt(int i)
{
    if(m_childs.count()<=i)
    {
        return NULL;
    }

    return m_childs.at(i);
}

void CDataStructure::appendChild(CDataStructure* child)
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"Append child"<<child->url()<<" to "<<url();
    m_childs.push_back(child);
}

void CDataStructure::removeChild(CDataStructure* child)
{
    if(!m_childs.contains(child))
    {
        return;
    }

    m_childs.removeOne(child);
}

void CDataStructure::write(QByteArray& data)
{
    m_data=data;
}

void CDataStructure::flush()
{
    for(int i=0; i<m_childs.count(); i++)
    {
        m_childs.at(i)->flush();
        delete m_childs.value(i);
    }
    m_data.clear();
}

bool CDataStructure::isFinished()
{
    if(!m_isDone)
    {
        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"Hi i'm stupied link and i'm not done "<<url();
        return false;
    }

    for(int i=0; i<m_childs.count();i++)
    {
        if(!m_childs.at(i)->isFinished())
        {
            return false;
        }
    }

    return true;
}

bool CDataStructure::contains(QUrl &url)
{
//    qDebug()<<"!!!!!!!!!"<<url<<" == "<<m_url;

    if(m_url==url)
    {
        return true;
    }

    for(int i=0; i<m_childs.count();i++)
    {
        if(m_childs.at(i)->contains(url))
        {
            return true;
        }
    }

    return false;
}

CDataStructure* CDataStructure::root()
{
    if(m_isRoot)
    {
        return this;
    }

    return m_root;
}

#include "cdatastructure.h"

CDataStructure::CDataStructure(QUrl url, QObject *parent) :
    QObject(parent)
{
    m_url=url;
}

CDataStructure::~CDataStructure()
{
    flush();
}

CDataStructure* CDataStructure::childAt(int i)
{
    if(m_childs.count()>=i)
    {
        return NULL;
    }

    return m_childs.at(i);
}

void CDataStructure::appendChild(CDataStructure* child)
{
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
    }
    m_data.clear();
}


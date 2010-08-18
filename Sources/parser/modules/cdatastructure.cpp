#include "cdatastructure.h"
#include <QDebug>

CDataStructure::CDataStructure(QUrl url, QObject *parent) :
    QObject(parent)
    ,m_isDone(false)
    ,m_isRoot(false)
    ,m_isRunned(false)
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


QStringList CDataStructure::findLinks(QList<QRegExp>& regexps)
{
    if(m_data.isEmpty())
    {
        return QStringList();
    }
    QStringList foundLinks;

    QString str(m_data);
    for(int i=0;i<regexps.count();i++)
    {
        QRegExp regexp=regexps.value(i);
        for(int pos=regexp.indexIn(str); pos>=0; pos=regexp.indexIn(str,pos+1))
        {
            if(foundLinks.contains(regexp.capturedTexts().at(0)))
            {
                continue;
            }

            foundLinks.append(regexp.capturedTexts().at(0));
        }
    }

    return foundLinks;
}

bool CDataStructure::isFinished()
{
    if(!m_isDone)
    {
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



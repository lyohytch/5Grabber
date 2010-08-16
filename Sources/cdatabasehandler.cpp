#include "cdatabasehandler.h"
#include <QDebug>
CDataBaseHandler::CDataBaseHandler(QObject *parent) :
    QObject(parent)
{
}

bool CDataBaseHandler::open(const QUrl &url)
{
    m_file.setFileName(url.host());
    return m_file.open(QIODevice::ReadWrite|QIODevice::Truncate);
}

qint64 CDataBaseHandler::write(const QByteArray& data)
{
    if(m_file.isOpen())
    {
        m_file.close();
    }
    m_file.open(QIODevice::ReadWrite|QIODevice::Truncate);
    m_file.write(data);
    m_file.flush();
    return data.size();
}

QByteArray CDataBaseHandler::read()
{
    m_file.seek(0);
    return m_file.readAll();
}

void CDataBaseHandler::close()
{
    m_file.close();
}

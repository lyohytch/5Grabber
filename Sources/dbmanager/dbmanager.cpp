#include "dbmanager.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QStringList>

DBmanager::DBmanager(): m_dbName("zakazrf"), m_dbHost("localhost"), m_dbUser("project"), m_dbPass("project_pass")
// DBmanager::DBmanager(const QString &db_name):m_dbName(db_name)
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName(m_dbHost);
    m_db.setDatabaseName(m_dbName);
    m_db.setUserName(m_dbUser);
    m_db.setPassword(m_dbPass);
}

DBmanager::~DBmanager()
{
    m_db.close();
}

bool DBmanager::init()
{
    m_status = m_db.open();
    qDebug() << "===> db open is " << m_status;
    qDebug() << m_db.tables(QSql::Tables);
    return m_status;
}

bool DBmanager::is_open()
{
    return m_status;
}

bool DBmanager::write(QVariantMap &data)
{
    Q_UNUSED(data);

    bool ok = false;
    if (m_status)
    {
        // EXAMPLE
//        for (int i = 1; i < 6; i++)
//        {
//            QSqlQuery query(m_db);
//            query.prepare("INSERT INTO Reduction VALUES (:id, :str, :num, '2010-09-11 21:01:17');");
//            query.bindValue(":id", QVariant::fromValue(11*i));
//            query.bindValue(":num", QVariant::fromValue(25*i));
//            QString str;
//            query.bindValue(":str", "abc" + str.setNum(i));
//            ok = query.exec();
//            if (!ok)
//            {
//                qDebug() << "===> qury error is: " << query.lastError().text();
//            }
//        }
    }
    return ok;
}

bool DBmanager::read()
{
    return true;
}

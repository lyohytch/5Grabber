#include "dbmanager.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QStringList>
#include <QDateTime>

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
    qDebug() << "===> db open is new" << m_status;
    if(m_status)
    {
        qDebug() << m_db.tables(QSql::Tables);
    }
    return m_status;
}

bool DBmanager::is_open()
{
    return m_status;
}

bool DBmanager::write(QVariantMap &data)
{
    Q_UNUSED(data);
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"***";
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

bool DBmanager::writeDoc(QVariantMap &data)
{
    bool ok = false;
    if (m_status)
    {
        QSqlQuery query(m_db);
        query.prepare("INSERT INTO DocFile VALUES (:id_file, :id_reduction, :name, :url, :uploaded, :last_parsed, :info);");
        query.bindValue(":id_file", QVariant());
        query.bindValue(":id_reduction", data.value("id_reduction"));
        query.bindValue(":name", QVariant(""));
        query.bindValue(":url", data.value("url"));
        query.bindValue(":uploaded", QVariant());
        query.bindValue(":last_parsed", QVariant(QDateTime::currentDateTime()));
        query.bindValue(":info", data.value("info"));
        ok = query.exec();
        if (!ok)
        {
            qDebug() << "===> qury error is: " << query.lastError().text();
        }
    }

    return true;
}

bool DBmanager::read()
{
    return true;
}

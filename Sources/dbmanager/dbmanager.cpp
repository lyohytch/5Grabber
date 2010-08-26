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
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<" :: "<<data.value("table");
    bool ok = false;
    if (m_status)
    {
        QSqlQuery query(m_db);
        if(data.value("table").toString() == "Status")
        {
            query.prepare("INSERT INTO Status VALUES (:id_status, :status);");
            query.bindValue(":id_status",data.value("id_status"));
            query.bindValue(":status",data.value("status"));
        }
        else if(data.value("table").toString() == "LOT")
        {
            query.prepare("INSERT INTO Lot VALUES (:num_lot, :id_reduction, :status, :subject, :id_contract, :obespechenie, :start_price, :best_price, :start_time, :protocol);");
            query.bindValue(":num_lot",data.value("num_lot"));
            query.bindValue(":id_reduction",data.value("id_reduction"));
            query.bindValue(":status",data.value("status"));
            query.bindValue(":subject",data.value("subject"));
            query.bindValue(":id_contract",data.value("id_contract"));
            query.bindValue(":obespechenie",data.value("obespechenie"));
            query.bindValue(":start_price",data.value("start_price"));
            query.bindValue(":best_price",data.value("best_price"));
            query.bindValue(":start_time",data.value("start_time"));
            query.bindValue(":protocol",data.value("protocol"));
        }
        else if(data.value("table").toString() == "Customer")
        {
            query.prepare("INSERT INTO Custoner VALUES (:id_customer, :name, :adress, :post_adress, :email, :telephone);");
            query.bindValue(":id_customer",data.value("id_customer"));
            query.bindValue(":name",data.value("name"));
            query.bindValue(":adress",data.value("adress"));
            query.bindValue(":post_adress",data.value("post_adress"));
            query.bindValue(":email",data.value("email"));
            query.bindValue(":telephone",data.value("telephone"));
        }
        else if(data.value("table").toString() == "Reduction")
        {
            query.prepare("INSERT INTO Reduction VALUES (:id_reduction, :string_number, :id_customer, :date_registration);");
            query.bindValue(":id_reduction",data.value("id_reduction"));
            query.bindValue(":string_number",data.value("string_number"));
            query.bindValue(":id_customer",data.value("id_customer"));
            query.bindValue(":date_registration",data.value("date_registration"));

        }
        else
        {
            qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<" Table doesn't exist";
        }
        ok = query.exec();
        if (!ok)
        {
            qDebug() << "===> qury error is: " << query.lastError().text();
        }
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

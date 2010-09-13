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
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<" :: "<<data.value("table");
    qDebug() << "##############################################################";
    qDebug() << data;
    qDebug() << "##############################################################";
    bool ok = false;
    if (m_status)
    {
        QSqlQuery query(m_db);
        if(data.value("table").toString() == "Status")
        {
            query.prepare("REPLACE INTO Status VALUES (:id_status, :status);");
            query.bindValue(":id_status",data.value("id_status"));
            query.bindValue(":status",data.value("status"));
        }
        else if(data.value("table").toString() == "LOT")
        {
            query.prepare("REPLACE INTO Lot VALUES (:id_reduction, :num_lot, :id_lot, :url, :status, :subject,"
                          " :obespechenie, :start_price, :best_price, :start_time, :end_time,"
                          " :winner, :protocol, :protocol_summary, :last_parsed);");
            query.bindValue(":id_reduction",data.value("id_reduction"));
            query.bindValue(":num_lot",data.value("num_lot"));
            query.bindValue(":id_lot",data.value("id_lot"));
            query.bindValue(":url",data.value("url"));
            query.bindValue(":status",data.value("status"));
            query.bindValue(":subject",data.value("subject"));
            //query.bindValue(":id_contract",data.value("id_contract"));
            query.bindValue(":obespechenie",data.value("obespechenie"));
            query.bindValue(":start_price",data.value("start_price"));
            query.bindValue(":best_price",data.value("best_price"));
            query.bindValue(":start_time",data.value("start_time"));
            query.bindValue(":end_time",data.value("end_time"));
            query.bindValue(":winner",data.value("winner"));
            query.bindValue(":protocol",data.value("protocol"));
            query.bindValue(":protocol_sumary",data.value("protocol_summary"));
            query.bindValue(":last_parsed",QVariant(QDateTime::currentDateTime()));
        }
        else if(data.value("table").toString() == "Customer")
        {
            query.prepare("REPLACE INTO Customer VALUES (:id_customer, :name, :adress, :post_adress, :email, :telephone);");
            query.bindValue(":id_customer",data.value("id_customer"));
            query.bindValue(":name",data.value("name"));
            query.bindValue(":adress",data.value("adress"));
            query.bindValue(":post_adress",data.value("post_adress"));
            query.bindValue(":email",data.value("email"));
            query.bindValue(":telephone",data.value("telephone"));
        }
        else if(data.value("table").toString() == "Reduction")
        {
            query.prepare("REPLACE INTO Reduction VALUES (:id_reduction, :string_number, :id_customer, "
                          " :date_registration, :url, :last_parsed);");
            query.bindValue(":id_reduction",data.value("id_reduction"));
            query.bindValue(":string_number",data.value("string_number"));
            query.bindValue(":id_customer",data.value("id_customer"));
            query.bindValue(":date_registration",data.value("date_registration"));
            query.bindValue(":url",data.value("url"));
            query.bindValue(":last_parsed", QVariant(QDateTime::currentDateTime()));

        }
        else if(data.value("table").toString() == "Participant")
        {
//            query.prepare("INSERT INTO Participant VALUES(:id_participant, :id_reduction, :num_lot, :name, :inn, :kpp);" );
            query.prepare("REPLACE INTO Participant VALUES(:id_participant, :name, :inn, :kpp, :id_reduction, :num_lot);" );
            query.bindValue(":id_participant",QVariant());
            //query.bindValue(":name", data.value("participants"));
            query.bindValue(":inn",QVariant());
            query.bindValue(":kpp",QVariant());
            query.bindValue(":id_reduction",data.value("id_reduction"));
            query.bindValue(":num_lot",data.value("num_lot"));
            int i = 0;
            foreach(QVariant t, data.value("participants").toList())
            {
//                //query.bindValue(":id_participant", i++);
                query.bindValue(":name", t);
                ok = query.exec();
                if(!ok)
                {
                    qDebug() << "===> query error is: " << query.lastError().text();
                }
                break;
            }
            return ok;

        }
        else if(data.value("table").toString() == "Auction")
        {
            query.prepare("REPLACE INTO Auction VALUES (:id_auction,"
                          " :code, :url, :subject, :customer_name, :customer_address, "
                          " :customer_post, :customer_email, :customer_phone, :customer_contact, "
                          " :auctionbegindate, :auctionenddate, :publicdate, :requestacceptdate, "
                          " :requestdate, :purchamount, :contrcoveramount, :contrcoveramountinpercent, "
                          " :contrneed, :status, :winner, :protocol);" );
            query.bindValue(":id_auction",data.value("id_reduction"));
            query.bindValue(":code",data.value("purchcode"));
            query.bindValue(":url",data.value("url"));
            query.bindValue(":subject",data.value("purchfullname"));
            query.bindValue(":customer_name",data.value("orgname"));
            query.bindValue(":customer_email",data.value("orgemail"));
            query.bindValue(":customer_phone",data.value("orgphones"));
            query.bindValue(":customer_post",data.value("orgpostaddress"));
            query.bindValue(":customer_address",data.value("orgplace"));
            query.bindValue(":customer_contact",data.value("orgcontactperson"));
            query.bindValue(":auctionbegindate",data.value("auctionbegindate"));
            query.bindValue(":auctionenddate",data.value("auctionenddate"));
            query.bindValue(":publicdate",data.value("publicdate"));
            query.bindValue(":requestacceptdate",data.value("requestacceptdate"));
            query.bindValue(":requestdate",data.value("requestdate"));
            query.bindValue(":purchamount",data.value("purchamount"));
            query.bindValue(":contrcoveramount",data.value("contrcoveramount"));
            query.bindValue(":contrcoveramountinpercent",data.value("contrcoveramountinpercent"));
            query.bindValue(":contrneed",data.value("contrneed"));
            query.bindValue(":status",QVariant());
            query.bindValue(":winner",data.value("winner"));
            query.bindValue(":protocol",data.value("protocol"));
        }
        else
        {
            qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<" Table doesn't exist";
            return ok;
        }
        ok = query.exec();
        if (!ok)
        {
            qDebug() << "===> query error is: " << query.lastError().text();
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
        query.prepare("REPLACE INTO DocFile VALUES (:id_file, :id_reduction, :num_lot, :name, :url, :uploaded, :last_parsed, :info);");
        query.bindValue(":id_file", data.value("id_file"));
        query.bindValue(":id_reduction", data.value("id_reduction"));
        query.bindValue(":num_lot", data.value("num_lot"));
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

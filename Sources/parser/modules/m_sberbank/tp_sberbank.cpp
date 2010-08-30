#include "tp_sberbank.h"

#include <QDebug>
#include <QMutex>


TP_sberbank::TP_sberbank()
{
    m_threadCounter = 0;
    m_signaller = new CParseSignaller();
}

TP_sberbank::~TP_sberbank()
{
    delete m_signaller;
}

bool TP_sberbank::init(int maxThreads, CDataStructure *data, DBmanager* db)
{
    m_maxThreads = maxThreads;
    m_data = data;
//    m_db->init();
    m_db = db;
    return TRUE;
}

CParseSignaller* TP_sberbank::signaller()
{
    return m_signaller;
}

bool TP_sberbank::run()
{
    QMutex mutex;
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<< "RUN PARSE TASK!!!";

    QVariantMap info = parse(m_data,true);
    for (int i = 0; i < m_data->childscCount(); i++)
    {
        if (m_data->childAt(i)->type() == CDataStructure::eDataTypeAuctionPage)
        {
            info.unite(parse(m_data->childAt(i),false));
        }
    }


    QVariantMap db_data;
    db_data.insert("info", info);
    db_data.insert("url", m_data->url().toString());
    db_data.insert("id_reduction", m_data->root()->url().toString().section("=", 1));
    mutex.lock();
        m_db->write(db_data);
    mutex.unlock();

    m_signaller->onParseFinished();
    return TRUE;
}

QVariantMap TP_sberbank::parse(CDataStructure* data,bool isRoot)
{

    QTextStream stream(data->read());
    QString html = stream.readAll();
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<< "PARSER";

    QVariantMap info;
    if(isRoot)
    {
        info.insert("date",QVariant(getDateData(html)));
        info.insert("code",QVariant(getCodeData(html)));
        info.insert("customer",QVariant(getCustomerData(html)));
        info.insert("providersmap",QVariant(""));
        info.insert("price",QVariant(getPriceData(html)));
        info.insert("providing",QVariant(getProvidingData(html)));
        return info;
    }
    else
    {
        QVariantMap temp = getProviderData(html);
        if(!temp.isEmpty())
        {
            info.insert("providersmap",QVariant(temp));
        }
        return info;
    }
}


QString TP_sberbank::getDataTeg(const QString& html,const QString& teg)
{
    QString data = html.section(teg,1,1,QString::SectionCaseInsensitiveSeps).trimmed();
    return data.remove(data.count() - 1,data.count());
}
QString TP_sberbank::getDataClearTeg(const QString& html,const QString& teg)
{
    QString data = getDataTeg(html,teg + "&gt;");
    data.remove("&lt;");
    data.remove("&quot;");
    return data.trimmed();
}
QString TP_sberbank::getDataClearTeg(const QString& html,const QString& teg, int number)
{
    QString data = html.section(teg + "&gt;",number,number,QString::SectionCaseInsensitiveSeps).trimmed();
    data = data.remove(data.count() - 1,data.count());
    data.remove("&lt;");
    data.replace(QString("&quot;"),QString("\""),Qt::CaseInsensitive);
    return data.trimmed();
}
int TP_sberbank::getCountTeg(const QString& html,const QString& teg)
{
   return html.count(teg + "&gt;",Qt::CaseInsensitive);
}

QVariantMap TP_sberbank::getDateData(const QString& html)
{
    QVariantMap  data;
    QString html_data = getDataTeg(html,"textarea");
    data.insert("publicdate", QVariant(getDataClearTeg(html_data,"publicdate")) );
    data.insert("requestdate", QVariant(getDataClearTeg(html_data,"requestdate") + " " +  getDataClearTeg(html_data,"requestdatetime")) );
    data.insert("requestacceptdate", QVariant(getDataClearTeg(html_data,"requestacceptdate")) );
    data.insert("auctionbegindate", QVariant(getDataClearTeg(html_data,"auctionbegindate")  + " " +  getDataClearTeg(html_data,"auctionbegindatetime")) );
    data.insert("auctionenddate", QVariant(getDataClearTeg(html_data,"auctionenddate")) );

    return data;
}

QVariantMap TP_sberbank::getCodeData(const QString& html)
{
    QVariantMap  data;
    QString html_data = getDataTeg(html,"textarea");
    data.insert("purchcode", QVariant(getDataClearTeg(html_data,"purchcode")) );
    return data;
}

QVariantMap TP_sberbank::getCustomerData(const QString& html)
{
    QVariantMap  data;
    QString html_data = getDataTeg(html,"textarea");

    data.insert("orgname", QVariant(getDataClearTeg(html_data,"orgname")));
    data.insert("orgtype", QVariant(getDataClearTeg(html_data,"orgtype")));
    data.insert("orgplace", QVariant(getDataClearTeg(html_data,"orgplace")));
    data.insert("orgpostaddress", QVariant(getDataClearTeg(html_data,"orgpostaddress")));
    data.insert("orgemail", QVariant(getDataClearTeg(html_data,"orgemail")));
    data.insert("orgphones", QVariant(getDataClearTeg(html_data,"orgphones")));
    data.insert("orgcontactperson", QVariant(getDataClearTeg(html_data,"orgcontactperson")));
    data.insert("custorgname", QVariant(getDataClearTeg(html_data,"custorgname")));
    return data;
}

QVariantMap TP_sberbank::getProviderData(const QString& html)
{
    QVariantMap  data;
    QString html_data = getDataTeg(html,"textarea");

    int countProvider  = getCountTeg(html_data,"reqno");
    int num = 0;
    for(int i = 1; i < countProvider; i = i + 2)
    {
        QVariantMap  provider;
        provider.insert("reqno",QVariant(getDataClearTeg(html_data,"reqno",i)));
        provider.insert("bufullname",QVariant(getDataClearTeg(html_data,"bufullname",i)));
        provider.insert("offerprice",QVariant(getDataClearTeg(html_data,"offerprice",i)));
        provider.insert("offerkind",QVariant(getDataClearTeg(html_data,"offerkind",i)));
        provider.insert("createat",QVariant(getDataClearTeg(html_data,"createat",i)));

        if( provider.value("reqno").toString().isEmpty() &&
            provider.value("bufullname").toString().isEmpty() &&
            provider.value("offerprice").toString().isEmpty() &&
            provider.value("offerkind").toString().isEmpty() &&
            provider.value("createat").toString().isEmpty() )
        {
            break;
        }

        data.insert( provider.value("reqno").toString(),QVariant(provider));
        num ++;
    }

    return data;
}

QVariantMap TP_sberbank::getPriceData(const QString& html)
{
    QVariantMap  data;
    QString html_data = getDataTeg(html,"textarea");
    data.insert("purchamount", QVariant(getDataClearTeg(html_data,"contrneed")));
    data.insert("purchpricestepmin", QVariant(getDataClearTeg(html_data,"purchpricestepmin")));
    data.insert("purchpricestepmax", QVariant(getDataClearTeg(html_data,"purchpricestepmax")));
    return data;
}

QVariantMap TP_sberbank::getProvidingData(const QString& html)
{
    QVariantMap  data;
    QString html_data = getDataTeg(html,"textarea");
    data.insert("contrneed", QVariant(getDataClearTeg(html_data,"contrneed")));
    data.insert("contrcoveramountinpercent", QVariant(getDataClearTeg(html_data,"contrcoveramountinpercent")));
    data.insert("contrcoveramount", QVariant(getDataClearTeg(html_data,"contrcoveramount")));
    data.insert("controreder", QVariant(getDataClearTeg(html_data,"controreder")));
    return data;
}


Q_EXPORT_PLUGIN2(TP_sberbank, TP_sberbank);


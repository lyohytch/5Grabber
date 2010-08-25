#include "tp_sberbank.h"

#include <QDebug>


TP_sberbank::TP_sberbank()
{
    m_threadCounter = 0;
    m_signaller = new CParseSignaller();
}

TP_sberbank::~TP_sberbank()
{
    delete m_signaller;
}

bool TP_sberbank::init(int maxThreads, CDataStructure *data)
{
    m_maxThreads = maxThreads;
    m_data = data;
    m_db->init();
    return TRUE;
}

CParseSignaller* TP_sberbank::signaller()
{
    return m_signaller;
}

bool TP_sberbank::run()
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<< "RUN PARSE TASK!!!";

    for (int i = 0; i < m_data->childscCount(); i++)
    {
        if (m_data->childAt(i)->type() == CDataStructure::eDataTypePage)
        {
            parse(m_data->childAt(i)->read());
        }
    }

    m_signaller->onParseFinished();
    return TRUE;
}

void TP_sberbank::parse(const QByteArray& _data)
{
    QTextStream stream(_data);
    QString html = stream.readAll();
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<< "RUN PARSE TASK!!!";



    QVariantMap db_data;
    //db_data.insert("info", info);
    //db_data.insert("url", m_data->url().toString());
    //db_data.insert("id_reduction", m_data->root()->url().toString().section("=", 1));
    m_db->write(db_data);
}


QString TP_sberbank::getDataTeg(const QString& html,const QString& teg)
{
    QString data = html.section(teg,1,1,QString::SectionCaseInsensitiveSeps).trimmed();
    return data.remove(data.count() - 1,data.count());
}
QString TP_sberbank::getDataClearTeg(const QString& html,const QString& teg)
{
    QString data = getDataTeg(html,teg);
    data.remove("&lt");
    data.remove("&quot");
    data.remove(";");
    return data.trimmed();
}

QMap<QString,QString> TP_sberbank::getDateData(const QString& html)
{
    QMap<QString,QString>  data;
    QString html_data = getDataTeg(html,"textarea");
    data.insert("publicdate", getDataClearTeg(html_data,"publicdate&gt") );
    data.insert("requestdate", getDataClearTeg(html_data,"requestdate&gt") + " " +  getDataClearTeg(html_data,"requestdatetime&gt") );
    data.insert("requestacceptdate", getDataClearTeg(html_data,"requestacceptdate&gt") );
    data.insert("auctionbegindate", getDataClearTeg(html_data,"auctionbegindate&gt")  + " " +  getDataClearTeg(html_data,"auctionbegindatetime&gt") );
    data.insert("auctionenddate", getDataClearTeg(html_data,"auctionenddate&gt") );

    return data;
}

QMap<QString,QString> TP_sberbank::getCodeData(const QString& html)
{
    QMap<QString,QString>  data;
    QString html_data = getDataTeg(html,"textarea");
    data.insert("purchcode", getDataClearTeg(html_data,"purchcode&gt") );
    return data;
}

QMap<QString,QString> TP_sberbank::getCustomerData(const QString& html)
{
    QMap<QString,QString>  data;
    QString html_data = getDataTeg(html,"textarea");

    data.insert("orgname", getDataClearTeg(html_data,"orgname&gt"));
    data.insert("orgtype", getDataClearTeg(html_data,"orgtype&gt"));
    data.insert("orgplace", getDataClearTeg(html_data,"orgplace&gt"));
    data.insert("orgpostaddress", getDataClearTeg(html_data,"orgpostaddress&gt"));
    data.insert("orgemail", getDataClearTeg(html_data,"orgemail&gt"));
    data.insert("orgphones", getDataClearTeg(html_data,"orgphones&gt"));
    data.insert("orgcontactperson", getDataClearTeg(html_data,"orgcontactperson&gt"));
    data.insert("custorgname", getDataClearTeg(html_data,"custorgname&gt"));
    return data;
}

QMap<QString,QString> TP_sberbank::getProviderData(const QString& html)
{
    QMap<QString,QString>  data;
    //QString html_data = getDataTeg(html,"textarea");
    //data.insert("purchdescr", getDataClearTeg(html_data,"purchdescr&gt"));
    return data;
}

QMap<QString,QString> TP_sberbank::getPriceData(const QString& html)
{
    QMap<QString,QString>  data;
    QString html_data = getDataTeg(html,"textarea");
    data.insert("purchamount", getDataClearTeg(html_data,"contrneed&gt"));
    data.insert("purchpricestepmin", getDataClearTeg(html_data,"purchpricestepmin&gt"));
    data.insert("purchpricestepmin", getDataClearTeg(html_data,"purchpricestepmax&gt"));
    return data;
}

QMap<QString,QString> TP_sberbank::getProvidingData(const QString& html)
{
    QMap<QString,QString>  data;
    QString html_data = getDataTeg(html,"textarea");
    data.insert("contrneed", getDataClearTeg(html_data,"contrneed&gt"));
    data.insert("contrcoveramountinpercent", getDataClearTeg(html_data,"contrcoveramountinpercent&gt"));
    data.insert("contrcoveramount", getDataClearTeg(html_data,"contrcoveramount&gt"));
    data.insert("controreder", getDataClearTeg(html_data,"controreder&gt"));
    return data;
}


Q_EXPORT_PLUGIN2(TP_sberbank, TP_sberbank);


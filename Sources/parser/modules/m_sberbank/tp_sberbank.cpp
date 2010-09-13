#include "tp_sberbank.h"

#include <QDebug>
#include <QMutex>
#include <QRegExp>
#include <QDateTime>


TP_sberbank::TP_sberbank()
{
    m_threadCounter = 0;
//    m_signaller = new CParseSignaller();
}

TP_sberbank::~TP_sberbank()
{
//    delete m_signaller;
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
        if (m_data->childAt(i)->type() == CDataStructure::eDataEventPage)
        {
            info.unite(parse(m_data->childAt(i),false));
        }
    }

    QVariantMap db_data;
    db_data.unite(info);
    db_data.insert("url", m_data->url().toString());
    db_data.insert("id_reduction", m_data->root()->url().toString().section("=", 1));
    db_data.insert("table", "Auction");
    mutex.lock();
        m_db->write(db_data);
    mutex.unlock();

//    m_signaller->onParseFinished();
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
//        info.insert("date",QVariant(getDateData(html)));
//        info.insert("code",QVariant(getCodeData(html)));
//        info.insert("customer",QVariant(getCustomerData(html)));
//        info.insert("providersmap",QVariant(""));
//        info.insert("price",QVariant(getPriceData(html)));
//        info.insert("providing",QVariant(getProvidingData(html)));
        info.unite(getDateData(html));
        info.unite(getCodeData(html));
        info.unite(getCustomerData(html));
        info.unite(getPriceData(html));
        info.unite(getProvidingData(html));
        return info;
    }
    else
    {
//        QVariantMap temp = getProviderData(html);
//        if(!temp.isEmpty())
//        {
////            info.insert("providersmap",QVariant(temp));
//            info.unite(temp);
//        }

        QString words = QString::fromUtf8("Сведения вторых частей заявок на участие в открытом аукционе в электронной форме");
        int position = html.indexOf(words);
        if (position != -1)
        {
            info.insert("protocol", data->url().toString());
            QString source = html.mid(position);
            source = source.remove(QRegExp("\n|\t|\r|\a"));
            //QString expr = QString::fromUtf8("<td>\\s*\\d{1,2}\\s*</td>\\s*<td>\\s*\\b(\\D*)\\b</td>\\s*<td>\\s*\\d*\\.{0,1}\\d{0,2}\\s*</td>\\s*<td>\\s*понижение\\s*</td>");
            QString expr = QString::fromUtf8("<tr>\\s*"
                                             "<td>\\s*\\d{1,2}\\s*</td>\\s*"
                                             "<td>(\\D*\\d*\\D*\\d*)</td>\\s*"
                                             "<td>\\s*\\d*.*\\d*\\s*</td>\\s*"
                                             "<td>\\D*</td>\\s*"
                                             "<td>\\s*\\d*.*\\d*.*\\d*\\s*\\d*:*\\d*:*\\d*\\s*</td>\\s*"
                                             "<td>\\s*соответствует\\s*</td>\\s*"
                                             "<td>\\w*</td>\\s*"
                                             "</tr>");
            QRegExp regexp(expr, Qt::CaseInsensitive);
            for (int pos = regexp.indexIn(source); pos > 0; pos = regexp.indexIn(source, pos + 1))
            {
                if (!regexp.cap(1).isEmpty())
                {
                    info.insert("winner", regexp.cap(1));
                    return info;
                }
            }
            info.insert("winner", QString());
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
    data.remove("&#171;");
    data.remove("&#187;");
    data.remove("\n");
    data.remove(";");
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
    data.insert("publicdate", QDateTime::fromString(getDataClearTeg(html_data,"publicdate"), QString("d.M.yyyy H:m")));
    data.insert("requestdate",QDateTime::fromString(getDataClearTeg(html_data,"requestdate") +
                                                    " " +  getDataClearTeg(html_data,"requestdatetime"),
                                                    QString("d.M.yyyy H:m")) );
    data.insert("requestacceptdate", QDate::fromString(getDataClearTeg(html_data,"requestacceptdate"), QString("d.M.yyyy")) );
    data.insert("auctionbegindate", QDateTime::fromString(getDataClearTeg(html_data,"auctionbegindate")  +
                                                          " " +  getDataClearTeg(html_data,"auctionbegindatetime"),
                                                          QString("d.M.yyyy H:m")) );
    data.insert("auctionenddate", QDateTime::fromString(getDataClearTeg(html_data,"auctionenddate"), QString("d.M.yyyy H:m")) );

    return data;
}

QVariantMap TP_sberbank::getCodeData(const QString& html)
{
    QVariantMap  data;
    QString html_data = getDataTeg(html,"textarea");
    data.insert("purchcode", getDataClearTeg(html_data,"purchcode") );
    data.insert("purchstate", getDataClearTeg(html_data,"purchstate"));
    return data;
}

QVariantMap TP_sberbank::getCustomerData(const QString& html)
{
    QVariantMap  data;
    QString html_data = getDataTeg(html,"textarea");

    data.insert("orgname", getDataClearTeg(html_data,"orgname"));
    data.insert("orgtype", getDataClearTeg(html_data,"orgtype"));
    data.insert("orgplace", getDataClearTeg(html_data,"orgplace"));
    data.insert("orgpostaddress", getDataClearTeg(html_data,"orgpostaddress"));
    data.insert("orgemail", getDataClearTeg(html_data,"orgemail"));
    data.insert("orgphones", getDataClearTeg(html_data,"orgphones"));
    data.insert("orgcontactperson", getDataClearTeg(html_data,"orgcontactperson"));
    data.insert("custorgname", getDataClearTeg(html_data,"custorgname"));
    data.insert("purchfullname", getDataClearTeg(html_data,"purchfullname"));
    return data;
}

QVariantMap TP_sberbank::getProviderData(const QString& html)
{
    QVariantMap  data;
    QString html_data = getDataTeg(html,"textarea");

    //int countProvider  = getCountTeg(html_data,"reqno");
    //int num = 0;
//    for(int i = 1; i < countProvider; i = i + 2)
//    {
        QVariantMap  provider;
        provider.insert("reqno",getDataClearTeg(html_data,"reqno"/*,i*/));
        provider.insert("bufullname",getDataClearTeg(html_data,"bufullname"/*,i*/));
        provider.insert("offerprice",getDataClearTeg(html_data,"offerprice"/*,i*/));
        provider.insert("offerkind",getDataClearTeg(html_data,"offerkind"/*,i*/));
        provider.insert("createat",getDataClearTeg(html_data,"createat"/*,i*/));

//        if( provider.value("reqno").toString().isEmpty() &&
//            provider.value("bufullname").toString().isEmpty() &&
//            provider.value("offerprice").toString().isEmpty() &&
//            provider.value("offerkind").toString().isEmpty() &&
//            provider.value("createat").toString().isEmpty() )
//        {
//            break;
//        }

        //data.insert( provider.value("reqno").toString(),QVariant(provider));
        //num ++;
//    }

//    return data;
    return provider;
}

QVariantMap TP_sberbank::getPriceData(const QString& html)
{
    QVariantMap  data;
    QString html_data = getDataTeg(html,"textarea");
    data.insert("purchamount", getDataClearTeg(html_data,"purchamount"));
    data.insert("purchpricestepmin", getDataClearTeg(html_data,"purchpricestepmin"));
    data.insert("purchpricestepmax", getDataClearTeg(html_data,"purchpricestepmax"));
    return data;
}

QVariantMap TP_sberbank::getProvidingData(const QString& html)
{
    QVariantMap  data;
    QString html_data = getDataTeg(html,"textarea");
    data.insert("contrneed", getDataClearTeg(html_data,"contrneed"));
    data.insert("contrcoveramountinpercent", getDataClearTeg(html_data,"contrcoveramountinpercent"));
    data.insert("contrcoveramount", getDataClearTeg(html_data,"contrcoveramount"));
    data.insert("controreder", getDataClearTeg(html_data,"controreder"));
    return data;
}


Q_EXPORT_PLUGIN2(TP_sberbank, TP_sberbank);


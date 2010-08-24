#include "tp_zakazrf.h"

#include <QDebug>
#include <QByteArray>

//Reduction
enum
{
    NumberLabel,
    PublicationDateLabel,
    CustomerLabel,
    CustomerPlaceLabel,
    CustomerPostAddressLabel,
    CustomerEMailLabel,
    CustomerContactPhoneLabel,
    noId
};

//Lot

enum
{
    Content_NumberLabel,
    Content_StageLabel,
    Content_SubjectLabel,
    Content_MaintenanceSumLabel,
    Content_FinalPriceLabel,
    Content_TradeBeginDateLabel
};

TP_zakazrf::TP_zakazrf()
{
    m_threadCounter = 0;
    m_signaller = new CParseSignaller();
    m_db = new DBmanager();
    //Reduction
    m_ids_Auc<<"ctl00_Content_ReductionViewForm_NumberLabel"  //Номер аукциона
             <<"ctl00_Content_ReductionViewForm_PublicationDateLabel" //Дата регистрации аукциона
    //Customer
             <<"ctl00_Content_ReductionViewForm_CustomerLabel"  //Имя заказчика
             <<"ctl00_Content_ReductionViewForm_CustomerPlaceLabel"  //Адрес заказчика
             <<"ctl00_Content_ReductionViewForm_CustomerPostAddressLabel"  //Почтовый адрес заказчика
             <<"ctl00_Content_ReductionViewForm_CustomerEMailLabel"  //Электронный адрес заказчика
             <<"ctl00_Content_ReductionViewForm_CustomerContactPhoneLabel"; //Контактный телефон
    //Lot
    m_ids_Lot<<"ctl00_Content_NumberLabel" //Номер лота
             <<"ctl00_Content_StageLabel" // Статус
             <<"ctl00_Content_SubjectLabel" //Предмет
             <<"ctl00_Content_MaintenanceSumLabel" //Размер обеспечения
             <<"ctl00_Content_FinalPriceLabel" //Лучшая цена
             <<"ctl00_Content_TradeBeginDateLabel"; // Дата проведения торгов в электр форме


}

TP_zakazrf::~TP_zakazrf()
{
    delete m_signaller;
    delete m_db;
}

bool TP_zakazrf::init(int maxThreads, CDataStructure *data)
{
    m_maxThreads = maxThreads;
    m_data = data;
    m_db->init();
    return TRUE;
}

CParseSignaller* TP_zakazrf::signaller()
{
    return m_signaller;
}

bool TP_zakazrf::run()
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<< "RUN PARSE TASK!!!" << m_data->childscCount();
    //QStringList addToDBList;
    //Парсим аукцион
    html_to_db(m_data->root(), m_ids_Auc, false);
    //Парсим лоты
    for (int i = 0; i < m_data->childscCount(); i++)
    {
        if (m_data->childAt(i)->type() == CDataStructure::eDataTypePage)
        {
            qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<m_data->childAt(i)->url().toString();
            html_to_db(m_data->childAt(i), m_ids_Lot, true);
        }
    }

    m_signaller->onParseFinished();
    return TRUE;

}

void  TP_zakazrf::html_to_db(CDataStructure *p_data, const QStringList &m_ids, bool isLot)
{
    // TODO initial variant of parser
    QStringList info = findProviding(p_data->read(),m_ids);
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<< "FINDINGS\n" <<info<<"\n"<<isLot;
    QVariantMap db_data;

    if(isLot)
    {
        //Write in Status table
        db_data.clear();
        db_data.insert("table","Status");
        db_data.insert("id_status", (p_data->url().toString()).section("=", 1));
        db_data.insert("status", info[Content_StageLabel]);
        m_db->write(db_data);

        //Write in Lot table
        db_data.clear();
        db_data.insert("table","LOT");
        db_data.insert("num_lot",info[Content_NumberLabel]);
        db_data.insert("id_reduction",p_data->root()->url().toString().section("=",1));
        db_data.insert("status", (p_data->url().toString()).section("=", 1));
        db_data.insert("subject", info[Content_SubjectLabel]);
        db_data.insert("id_contract", (p_data->url().toString()).section("=", 1));
        db_data.insert("obespechenie", info[Content_MaintenanceSumLabel]);
        db_data.insert("start_price", info[Content_FinalPriceLabel]);
        db_data.insert("best_price", info[Content_FinalPriceLabel]);
        db_data.insert("start_time", info[Content_TradeBeginDateLabel]);
        db_data.insert("protocol","");
        m_db->write(db_data);
    }
    //Customer and Reduction tables
    else
    {
        //Write in Customer Table
        db_data.clear();
        db_data.insert("table","Customer");
        db_data.insert("id_customer", (p_data->url().toString()).section("=", 1));
        db_data.insert("name", info[CustomerLabel]);
        db_data.insert("adress",info[CustomerPlaceLabel]);
        db_data.insert("post_adress", info[CustomerPostAddressLabel]);
        db_data.insert("email", info[CustomerEMailLabel]);
        db_data.insert("telephone", info[CustomerContactPhoneLabel]);
        m_db->write(db_data);

        //Write in Reduction Table
        db_data.clear();
        db_data.insert("table", "Reduction");
        db_data.insert("id_reduction", (p_data->url().toString()).section("=", 1));
        db_data.insert("string_number", info[NumberLabel]);
        db_data.insert("id_customer", (p_data->url().toString()).section("=", 1));
        db_data.insert("date_registration", info[PublicationDateLabel]);
        m_db->write(db_data);
    }
}

QStringList TP_zakazrf::findProviding(const QByteArray &source, const QStringList &a_ids)
{
    QTextStream stream(source);
    QString sourceStr(stream.readAll());
    sourceStr = sourceStr.remove(QRegExp("\n|\t|\r|\a"));
    QStringList appToDB;

    //QRegExp regexp(QString("<[^<]*>[^<]*</[^<]*>"), Qt::CaseInsensitive);
    //for (int pos = regexp.indexIn(sourceStr); pos >= 0; pos = regexp.indexIn(sourceStr,pos + 1))
    //{
    //    QString tmp = regexp.capturedTexts().at(0);
    //    foreach(QString id,a_ids)
    //    {
    //       if (tmp.contains(id,Qt::CaseInsensitive))
    //       {
    //            appToDB.append(extractFromSpanTag(tmp));
    //       }
    //    }
    //}
    //Ищем каждый айдишник в файлике
    foreach(QString id, a_ids)
    {
        QString retStr;
        int pos = sourceStr.lastIndexOf(id);
        while(sourceStr[pos++] != '>' && pos < sourceStr.length());
        while(sourceStr[pos] != '<' && pos < sourceStr.length())
        {
            retStr += sourceStr[pos];
            pos++;
        }
        if (retStr.isEmpty())
        {
            qDebug()<<Q_FUNC_INFO<<" Template not found";            
        }
        else
        {
            appToDB.append(retStr);
        }
    }

    return appToDB;

}

QString TP_zakazrf::extractFromSpanTag(const QString & tagTxt)
{
    QString retStr;
    int i = 0;
    while(tagTxt[i++] != '>' && i < tagTxt.length());
    while(tagTxt[i] != '<' && i < tagTxt.length())
    {
       retStr += tagTxt[i];
       i++;
    }

    return retStr;
}

Q_EXPORT_PLUGIN2(TP_zakazrf, TP_zakazrf);

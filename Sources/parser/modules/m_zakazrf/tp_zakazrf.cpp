#include "tp_zakazrf.h"

#include "constants.h"
#include <QByteArray>
#include <QStringList>
#include <QMutex>
#include <QDateTime>

//Reduction
const QString NumberLabel                 = "ctl00_Content_ReductionViewForm_NumberLabel";
const QString PublicationDateLabel        = "ctl00_Content_ReductionViewForm_PublicationDateLabel";
const QString CustomerLabel               = "ctl00_Content_ReductionViewForm_CustomerLabel";
const QString CustomerPlaceLabel          = "ctl00_Content_ReductionViewForm_CustomerPlaceLabel";
const QString CustomerPostAddressLabel    = "ctl00_Content_ReductionViewForm_CustomerPostAddressLabel";
const QString CustomerEMailLabel          = "ctl00_Content_ReductionViewForm_CustomerEMailLabel";
const QString CustomerContactPhoneLabel   = "ctl00_Content_ReductionViewForm_CustomerContactPhoneLabel";

//Lot

const QString Content_NumberLabel         = "ctl00_Content_NumberLabel";
const QString Content_StageLabel          = "ctl00_Content_StageLabel";
const QString Content_SubjectLabel        = "ctl00_Content_SubjectLabel";
const QString Content_MaintenanceSumLabel = "ctl00_Content_MaintenanceSumLabel";
const QString Content_FinalPriceLabel     = "ctl00_Content_FinalPriceLabel";
const QString Content_TradeBeginDateLabel = "ctl00_Content_TradeBeginDateLabel";
const QString labelLotParticipantCount    = "ctl00_Content_labelLotParticipantCount";

TP_zakazrf::TP_zakazrf()
{
    m_threadCounter = 0;
//    m_signaller = new CParseSignaller();
//    m_db = new DBmanager();
    //TODO сделать константами
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
             <<"ctl00_Content_TradeBeginDateLabel" // Дата проведения торгов в электр форме
             <<"ctl00_Content_labelLotParticipantCount";

    montheTable[QString::fromUtf8("января")] = "1";
    montheTable[QString::fromUtf8("февраля")] = "2";
    montheTable[QString::fromUtf8("марта")] = "3";
    montheTable[QString::fromUtf8("апреля")] = "4";
    montheTable[QString::fromUtf8("мая")] = "5";
    montheTable[QString::fromUtf8("июня")] = "6";
    montheTable[QString::fromUtf8("июля")] = "7";
    montheTable[QString::fromUtf8("августа")] = "8";
    montheTable[QString::fromUtf8("сентября")] = "9";
    montheTable[QString::fromUtf8("октября")] = "10";
    montheTable[QString::fromUtf8("ноября")] = "11";
    montheTable[QString::fromUtf8("декабря")] = "12";
}

TP_zakazrf::~TP_zakazrf()
{
//    delete m_signaller;
//    delete m_db;
}

bool TP_zakazrf::init(int maxThreads, CDataStructure *data, DBmanager* db)
{
    m_maxThreads = maxThreads;
    m_data = data;
//    m_db->init();
    m_db = db;
    return TRUE;
}

CParseSignaller* TP_zakazrf::signaller()
{
    return m_signaller;
}

bool TP_zakazrf::run()
{
    qDebug()<< "RUN PARSE TASK!!!" << m_data->childscCount();
    //Parsing auction
    html_to_db(m_data->root(), m_ids_Auc, false);
    //Parsing lots
    for (int i = 0; i < m_data->childscCount(); i++)
    {
        if (m_data->childAt(i)->type() == CDataStructure::eDataTypeLotPage)
        {
            qDebug()<<m_data->childAt(i)->url().toString();
            html_to_db(m_data->childAt(i), m_ids_Lot, true);
        }
    }

//    m_signaller->onParseFinished();
    return TRUE;

}

void  TP_zakazrf::html_to_db(CDataStructure *p_data, const QStringList &m_ids, bool isLot)
{
    QVariantMap info = findProviding(p_data,m_ids);
    QVariantMap db_data;
    QMutex mutex;

    if(isLot)
    {
        QString winner;
        //Getting participant information
        for(int i = 0; i < p_data->childscCount(); i++)
        {
            if (p_data->childAt(i)->type() == CDataStructure::eDataTypeLotStatisticPage)
            {
                db_data.clear();
                db_data.insert("table", "Participant");
                if( !info[labelLotParticipantCount].toString().isEmpty() )
                {
                    participantsCount = info[labelLotParticipantCount].toInt();
                }
                else
                {
                    participantsCount = 0;
                }
                db_data.insert("id_reduction",p_data->root()->root()->url().toString().section("=",1));
                db_data.insert("num_lot",info[Content_NumberLabel]);
                QStringList partNames = findParticipants(p_data->childAt(i)->read(), info[Content_FinalPriceLabel].toString());
                db_data.insert("participants",partNames);
                if (!partNames.isEmpty())
                {
                    winner = partNames.first();
                }
                mutex.lock();
                    m_db->write(db_data);
                mutex.unlock();
            }
        }

        //Write in Lot table
        db_data.clear();
        QString link = findProtocol(p_data);
        bool protocol_exists = false;
        for (int j = 0; j < p_data->childscCount(); j++)
        {
            if (!link.isEmpty() && p_data->childAt(j)->url().toString().contains(link))
            {
                db_data = db_data.unite(parseProtocol(p_data->childAt(j)));
                db_data.insert("protocol", p_data->childAt(j)->url().toString());
                protocol_exists = true;
                //qDebug() << db_data;
                break;
            }
        }
        if (!protocol_exists)
        {
            db_data.insert("protocol", QVariant());
            db_data.insert("end_time", QVariant());
        }
        db_data.insert("table","LOT");
        db_data.insert("num_lot",info[Content_NumberLabel]);
        db_data.insert("id_reduction",p_data->root()->url().toString().section("=",1));
        db_data.insert("id_lot",p_data->url().toString().section("=",1));
        db_data.insert("status", (p_data->url().toString()).section("=", 1));
        db_data.insert("subject", info[Content_SubjectLabel]);
        db_data.insert("id_contract", (p_data->url().toString()).section("=", 1));
        db_data.insert("obespechenie", info[Content_MaintenanceSumLabel]);
        db_data.insert("start_price", info["start_price"]);
        db_data.insert("best_price", info[Content_FinalPriceLabel]);
        db_data.insert("start_time", QDateTime::fromString(info.value(Content_TradeBeginDateLabel).toString(),
                                                           QString("d.M.yyyy H:m")));
        db_data.insert("winner", winner);
        db_data.insert("url", p_data->url().toString());
        mutex.lock();
            m_db->write(db_data);
        mutex.unlock();

        //Write in Status table
        db_data.clear();
        db_data.insert("table","Status");
        db_data.insert("id_status", p_data->url().toString().section("=",1));
        db_data.insert("status", info[Content_StageLabel]);
        mutex.lock();
            m_db->write(db_data);
        mutex.unlock();
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
        mutex.lock();
            m_db->write(db_data);
        mutex.unlock();

        //Write in Reduction Table
        db_data.clear();
        db_data.insert("table", "Reduction");
        db_data.insert("id_reduction", (p_data->url().toString()).section("=", 1));
        db_data.insert("string_number", info[NumberLabel]);
        db_data.insert("id_customer", (p_data->url().toString()).section("=", 1));
        db_data.insert("date_registration", info[PublicationDateLabel]);
        db_data.insert("url", p_data->root()->url().toString());
        mutex.lock();
            m_db->write(db_data);
        mutex.unlock();
    }
}

QStringList TP_zakazrf::findParticipants(const QByteArray &source, const QString &templ)
{
    int tmpCount = participantsCount;

    if(tmpCount <= 0) return QStringList();

    QTextStream stream(source);
    QString sourceStr(stream.readAll());
    sourceStr = sourceStr.remove(QRegExp("\n|\t|\r|\a"));

    //Getting winner
    QString winner;
    int pos = sourceStr.lastIndexOf(templ);
    if(pos == -1) return QStringList();

    //Сдвиг
    int c = 0;
    while(c < 2)
    {
        if( pos >= sourceStr.length())
        {
            winner.clear();
            break;
        }
        if(sourceStr[pos++] == '>')
        {
            c++;
        }
    }
    //Gettong winner's company name
    while(pos < sourceStr.length() && sourceStr[pos] != '<')
    {
        winner += sourceStr[pos];
        pos++;
    }
    QStringList retList;

    //Getting other participants
    retList.append(winner);

    ////BIG WORLAROUND START
    //*********************************
    tmpCount--;
    QString start = "<table";
    QString end   = "</table>";
    QString strStart = "<td";
    int posTable = sourceStr.lastIndexOf(start);
    int posEndTable = sourceStr.indexOf(end);
    //Взяли табличку
    QString tableStr = sourceStr.mid(posTable, posEndTable - posTable);
    //qDebug()<<Q_FUNC_INFO<<"::::"<<tableStr;
    //Можно сделать, так как структура таблицы проста и без вложений
    QRegExp regexp(QString("<[^<]*>[^<]*</[^<]*>"), Qt::CaseInsensitive);
    posTable = 0;
    int l = 0;
    for (int posL = regexp.indexIn(tableStr); posL >= 0; posL = regexp.indexIn(tableStr,posL + 1))
    {
        QString tmp = regexp.capturedTexts().at(0);
        if(tmp.contains(strStart))
        {
            l++;
            //TODO пропускаем два td считываем третий
           if(l==3)
            {
                l = 0;
                int p = 0;
                QString part;
                while(p < tmp.length() && tmp[p++] != '>');
                while(p < tmp.length() && tmp[p] != '<')
                {
                    part += tmp[p++];
                }
                if(retList.contains(part)) continue;
                retList.append(part);
                tmpCount--;
                if(tmpCount == 0) break;
            }
        }
    }
    ////BIG WORLAROUND END
    //*********************************

    return retList;
}

QVariantMap TP_zakazrf::findProviding(CDataStructure *p_data, const QStringList &a_ids)
{
    QTextStream stream(p_data->read());
    QString sourceStr(stream.readAll());
    sourceStr = sourceStr.remove(QRegExp("\n|\t|\r|\a"));
    QVariantMap appToDB;

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
        if(pos == -1 ) continue;
        while(pos < sourceStr.length() && sourceStr[pos++] != '>');
        while(pos < sourceStr.length() && sourceStr[pos] != '<')
        {
            retStr += sourceStr[pos];
            pos++;
        }
        if (retStr.isEmpty())
        {
            // TODO remove debug
            qDebug()<<" Template not found. id"<<id;
        }
        else
        {
            appToDB.insert(id,retStr);
        }
    }

//    QString endTimeKeyword;
//    endTimeKeyword=endTimeKeyword.fromUtf8("<td>Протокол проведения аукциона</td><td>");
//    qDebug()<<endTimeKeyword;
//    int endTimeStartIndex=sourceStr.lastIndexOf(endTimeKeyword);
//    if(endTimeStartIndex==-1)
//    {
//        return appToDB;
//    }
//
//    endTimeStartIndex+=endTimeKeyword.size();
//
//    QString retStr;
//    while(endTimeStartIndex < sourceStr.count() && sourceStr[endTimeStartIndex] != '<')
//    {
//        retStr+=sourceStr[endTimeStartIndex++];
//    }
//    if (!retStr.isEmpty())
//    {
//        appToDB.insert("lotEndTime",retStr);
//    }

    if (!p_data->isRoot())
    {
        QString expr;
        expr = QString::fromUtf8("Начальная\\sцена\\sлота\\s\\(контракта\\)\\s*</b>\\s*</td>\\s*<td>"
                                 "\\s*(\\d*\\D?\\d{0,2})\\s*");
        QRegExp regexp(expr, Qt::CaseInsensitive);
        for (int pos = regexp.indexIn(sourceStr); pos > 0; pos = regexp.indexIn(sourceStr, pos + 1))
        {
            appToDB.insert("start_price", regexp.cap(1).replace(",", ".").toFloat());
        }
    }

//    qDebug()<<retStr;
    return appToDB;

}

QString TP_zakazrf::extractFromSpanTag(const QString & tagTxt)
{
    QString retStr;
    int i = 0;
    while(i < tagTxt.length() && tagTxt[i++] != '>');
    while(i < tagTxt.length() && tagTxt[i] != '<' )
    {
       retStr += tagTxt[i];
       i++;
    }

    return retStr;
}

QString TP_zakazrf::findProtocol(CDataStructure* p_data)
{
    QTextStream stream(p_data->read());
    QString sourceStr(stream.readAll());
    sourceStr = sourceStr.remove(QRegExp("\n|\t|\r|\a"));
    QString expr = QString::fromUtf8("href=\\\"(\\D*id=\\d{3,6})\\\">Протокол\\sпроведения\\sаукциона");
    QRegExp regexp(expr, Qt::CaseInsensitive);
    QString link;

    for (int pos = regexp.indexIn(sourceStr); pos > 0; pos = regexp.indexIn(sourceStr, pos + 1))
    {
        link = regexp.cap(1);
        if (!link.isEmpty())
        {
            return link;
        }
    }
    return link;
}

QVariantMap TP_zakazrf::parseProtocol(CDataStructure *p_data)
{
    QTextStream stream(p_data->read());
    QString sourceStr(stream.readAll());
    sourceStr = sourceStr.remove(QRegExp("\n|\t|\r|\a"));
    QVariantMap result;
    QString list;
    QString expr = QString::fromUtf8("<p>\\s*Время\\sокончания\\sаукциона\\s*<u>\\s*((\\d{1,2}):(\\d{1,2}):(\\d{1,2}))\\s*</u>\\s*</p>");
    QRegExp regexp1(expr, Qt::CaseInsensitive);
//    QRegExp regexp1("<p>\\s*Время\\sокончания\\sаукциона\\s*<u>\\s*(\\d{1,2}):(\\d{1,2}):(\\d{1,2})\\s*</u>\\s*</p>", Qt::CaseInsensitive);
    for (int pos = regexp1.indexIn(sourceStr); pos > 0; pos = regexp1.indexIn(sourceStr, pos + 1))
    {
        if (!regexp1.cap().isEmpty())
        {
            list.append(regexp1.cap(1) + " ");
            break;
        }
    }
    expr = QString::fromUtf8("<p>\\s*Дата\\sокончания\\sаукциона\\s*<u>\\s*(\\d{1,2}\\s)(\\w*)(\\s\\d{4})\\s");
    QRegExp regexp2(expr, Qt::CaseInsensitive);
//    QRegExp regexp2("<p>\\s*Дата\\sокончания\\sаукциона\\s*<u>\\s*(\\d{1,2}\\s)(\\w*)(\\s\\d{4})\\s", Qt::CaseInsensitive);
    for (int pos = regexp2.indexIn(sourceStr); pos > 0; pos = regexp2.indexIn(sourceStr, pos + 1))
    {
        if (!regexp2.cap().isEmpty())
        {
            list.append(regexp2.cap(1));
            list.append(montheTable.value(regexp2.cap(2)));
            list.append(regexp2.cap(3));
            break;
        }
        break;
    }
    QDateTime dt = QDateTime::fromString(list, QString("H:m:s d M yyyy"));


    result.insert("end_time", dt);

//    expr = QString::fromUtf8("<p>\\s*4.Начальная\\s\\(максимальная\\)\\sцена\\sконтракта\\s\\(лота\\):\\s*<u>"
//                             "\\s*(\\d*\\D?\\d{0,2})\\s");
//    QRegExp regexp3(expr, Qt::CaseInsensitive);
//    for (int pos = regexp3.indexIn(sourceStr); pos > 0; pos = regexp3.indexIn(sourceStr, pos + 1))
//    {
//        result.insert("start_price", regexp3.cap(1).replace(",", ".").toFloat());
//    }
    return result;
}

Q_EXPORT_PLUGIN2(TP_zakazrf, TP_zakazrf);

#include "tp_zakazrf.h"

#include <QDebug>
#include <QByteArray>
//#include <QtWebKit/QWebPage>
//#include <QtWebKit/QWebFrame>
//#include <QtWebKit/QWebElement>

TP_zakazrf::TP_zakazrf()
{
    m_threadCounter = 0;
    m_signaller = new CParseSignaller();
    m_db = new DBmanager();
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

//QMap<QString,QVariant> TP_zakazrf::parse(CDataStructure* _data)
//{
//    QMap<QString,QVariant> result;
//    QString sdata(_data->read());
//    result = parse_html_auk(sdata);
//
//    for(int i = 0;  i < _data->childscCount(); i++)
//    {
//        CDataStructure* child= _data->childAt(i);
//        if(!child)
//        {
//            qCritical()<<"Fatal!!!! Childs is null";
//            continue;
//        }
//
//        switch(child->type())
//        {
//            case 0:
//            {
//                QString sdata(child->read());
//                result.unite(parse_html_lot(sdata));
//                break;
//            }
//            case 1:
//            {
//                break;
//            }
//            default:
//                break;
//        }
//    }
//
//    return result;
//}

bool TP_zakazrf::run()
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<< "RUN PARSE TASK!!!";

    for (int i = 0; i < m_data->childscCount(); i++)
    {
        if (m_data->childAt(i)->type() == CDataStructure::eDataTypePage)
        {
            html_to_db(m_data->childAt(i));
        }
    }

    m_signaller->onParseFinished();
    return TRUE;

}

QMap<QString,QVariant> TP_zakazrf::parse_html_auk(const QString& _html)
{
    //QString html_txt = html_to_txt(_html);
    QMap<QString,QVariant> result;
    result.insert("text1","aaa");
    return result;

}

QMap<QString,QVariant> TP_zakazrf::parse_html_lot(const QString& _html)
{
    //QString html_txt = html_to_txt(_html);
    QMap<QString,QVariant> result;
    result.insert("text2","bbb");
    return result;
}

void  TP_zakazrf::html_to_db(const CDataStructure *m_data)
{
    // TODO initial variant of parser
    QString info = findProviding(m_data->read());
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<< "FINDINGS\n" <<info;
    QVariantMap db_data;
    db_data.insert("info", info);
    db_data.insert("url", m_data->url().toString());
    //db_data.insert("id_reduction", m_data->root()->url().toString().section("=", 1));
    m_db->write(db_data);

}

QString TP_zakazrf::findProviding(const QByteArray &source)
{
    QTextStream stream(source);
    QString sourceStr(stream.readAll());
    sourceStr = sourceStr.remove(QRegExp("\n|\t|\r|\a"));
    QString found;
    bool isFound = false;
    QRegExp regexp(QString("<[^<]*>[^<]*<[^<]*>"), Qt::CaseInsensitive);
    for (int pos = regexp.indexIn(sourceStr); pos >= 0; pos = regexp.indexIn(sourceStr,pos + 1))
    {
        QString tmp = regexp.capturedTexts().at(0);
        /*
        //If you found "obespechenie" then you should read next word
        if (isFound)
        {
            isFound = false;
            found.append(tmp + "\n");
            continue;
        }
        */
        qDebug()<<__LINE__<<Q_FUNC_INFO<<" xx "<<tmp;
        if (tmp.contains(QTextStream("обеспечен").readAll(),Qt::CaseInsensitive))
        {
            isFound = true;
            found.append(tmp + " ");
        }
    }

    return found;
}

Q_EXPORT_PLUGIN2(TP_zakazrf, TP_zakazrf);

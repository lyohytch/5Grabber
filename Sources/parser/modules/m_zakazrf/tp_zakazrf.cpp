#include "tp_zakazrf.h"

#include <QDebug>
#include <QByteArray>
#include <QtWebKit/QWebPage>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>

TP_zakazrf::TP_zakazrf()
{
    m_threadCounter = 0;
    m_signaller = new CParseSignaller();
}

TP_zakazrf::~TP_zakazrf()
{
    delete m_signaller;
}

bool TP_zakazrf::init(int maxThreads, CDataStructure *data)
{
    m_maxThreads = maxThreads;
    m_data = data;
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
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<< parse_html_lot(QString(""));
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<< parse_html_auk(QString(""));
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

QString TP_zakazrf::html_to_txt(const QString& _html)
{
    QWebPage page(this);
    QWebFrame *frame = page.mainFrame();
    frame->setHtml(_html);
    QWebElement document = frame->documentElement();
    return document.toPlainText();
}

Q_EXPORT_PLUGIN2(TP_zakazrf, TP_zakazrf);

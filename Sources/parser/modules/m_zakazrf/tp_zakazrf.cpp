#include "tp_zakazrf.h"

#include <QDebug>
#include <QByteArray>
#include  <QtWebKit/QWebPage>
#include  <QtWebKit/QWebFrame>
#include  <QtWebKit/QWebElement>

QMap<QString,QVariant> TP_zakazrf::parse(CDataStructure* _data)
{
    QMap<QString,QVariant> result;
    QString sdata(_data->read());
    result = parse_html_auk(sdata);

    for(int i = 0;  i < _data->childscCount(); i++)
    {
        CDataStructure* child= _data->childAt(i);
        if(!child)
        {
            qCritical()<<"Fatal!!!! Chils is null";
            continue;
        }

        switch(child->type())
        {
            case 0:
            {
                QString sdata(child->read());
                result.unite(parse_html_lot(sdata));
                break;
            }
            case 1:
            {
                break;
            }
            default:
                break;
        }
    }

    return result;
}

QMap<QString,QVariant> TP_zakazrf::parse_html_auk(const QString& _html)
{
    QString html_txt = html_to_txt(_html);
    QMap<QString,QVariant> result;
    result.insert("text1","aaa");
    return result;

}

QMap<QString,QVariant> TP_zakazrf::parse_html_lot(const QString& _html)
{
    QString html_txt = html_to_txt(_html);
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

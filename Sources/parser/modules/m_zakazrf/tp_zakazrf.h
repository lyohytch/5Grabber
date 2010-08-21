#ifndef TP_ZAKAZRF_H
#define TP_ZAKAZRF_H

#include <tp_task.h>
#include <cdatastructure.h>

#include <QObject>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QtPlugin>

class TP_zakazrf : public QObject, public TP_Task
{
    Q_OBJECT
    Q_INTERFACES(TP_Task)
public:
    virtual QMap<QString,QVariant> parse(CDataStructure* _data);
private:
    QMap<QString,QVariant> parse_html_auk(const QString& _html);
    QMap<QString,QVariant> parse_html_lot(const QString& _html);

    QString html_to_txt(const QString& _html);
};

#endif
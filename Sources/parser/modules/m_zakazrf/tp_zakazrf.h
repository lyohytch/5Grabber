#ifndef TP_ZAKAZRF_H
#define TP_ZAKAZRF_H

#include <tp_task.h>
#include <cdatastructure.h>

#include <QObject>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QtPlugin>

#include "dbmanager.h"

class TP_zakazrf : public QObject, public TP_Task
{
    Q_OBJECT
    Q_INTERFACES(TP_Task)

public:
    TP_zakazrf();
    ~TP_zakazrf();

    virtual bool init(int maxThreads, CDataStructure* data);
    virtual bool run();
    //virtual QMap<QString,QVariant> parse(CDataStructure* _data);
    virtual CParseSignaller* signaller();

private:
    QMap<QString,QVariant> parse_html_auk(const QString& _html);
    QMap<QString,QVariant> parse_html_lot(const QString& _html);
    void html_to_db(const CDataStructure *);
    QString findProviding(const QByteArray &source);
    qint64 m_threadCounter;
    int m_maxThreads;
    CDataStructure *m_data;
    CParseSignaller* m_signaller;
    DBmanager* m_db;
};

#endif

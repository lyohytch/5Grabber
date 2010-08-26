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
    void html_to_db(CDataStructure *, const QStringList &m_ids, bool isLot); //хтмл в стринглист
    QVariantMap findProviding(const QByteArray &source, const QStringList &);
    qint64 m_threadCounter;
    int m_maxThreads;
    CDataStructure *m_data;
    CParseSignaller* m_signaller;
    DBmanager* m_db;
    QStringList m_ids_Auc;
    QStringList m_ids_Lot;
    QString extractFromSpanTag(const QString & tagTxt);
    int participantsCount;

    // todo
    QStringList findParticipants(const QByteArray &source, const QString &templ);

};

#endif

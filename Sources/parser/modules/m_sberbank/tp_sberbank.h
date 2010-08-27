#ifndef TP_SBERBANK_H
#define TP_SBERBANK_H

#include <tp_task.h>
#include <cdatastructure.h>
#include "dbmanager.h"

#include <QObject>
#include <QMap>
#include <QString>
#include <QByteArray>
#include <QVariant>
#include <QtPlugin>


class TP_sberbank : public QObject, public TP_Task
{
    Q_OBJECT
    Q_INTERFACES(TP_Task)
public:
    TP_sberbank();
    ~TP_sberbank();

    virtual bool init(int maxThreads, CDataStructure* data, DBmanager* db);
    virtual bool run();
    virtual CParseSignaller* signaller();

private:

    QString getDataTeg(const QString& html,const QString& teg);
    QString getDataClearTeg(const QString& html,const QString& teg);
    QString getDataClearTeg(const QString& html,const QString& teg, int number);
    int getCountTeg(const QString& html,const QString& teg);

    QVariantMap getDateData(const QString& html);
    QVariantMap getCodeData(const QString& html);
    QVariantMap getCustomerData(const QString& html);
    QVariantMap getProviderData(const QString& html);
    QVariantMap getPriceData(const QString& html);
    QVariantMap getProvidingData(const QString& html);

    QVariantMap parse(CDataStructure* data, bool isRoot);

    qint64 m_threadCounter;
    int m_maxThreads;
    CDataStructure *m_data;
    CParseSignaller* m_signaller;
    DBmanager* m_db;

};

#endif // TP_SBERBANK_H

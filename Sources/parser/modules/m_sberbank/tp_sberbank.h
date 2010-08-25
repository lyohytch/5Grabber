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

    virtual bool init(int maxThreads, CDataStructure* data);
    virtual bool run();
    virtual CParseSignaller* signaller();

private:

    QString getDataTeg(const QString& html,const QString& teg);
    QString getDataClearTeg(const QString& html,const QString& teg);

    QMap<QString,QString> getDateData(const QString& html);
    QMap<QString,QString> getCodeData(const QString& html);
    QMap<QString,QString> getCustomerData(const QString& html);
    QMap<QString,QString> getProviderData(const QString& html);
    QMap<QString,QString> getPriceData(const QString& html);
    QMap<QString,QString> getProvidingData(const QString& html);

    void parse(const QByteArray& _data);

    qint64 m_threadCounter;
    int m_maxThreads;
    CDataStructure *m_data;
    CParseSignaller* m_signaller;
    DBmanager* m_db;

};

#endif // TP_SBERBANK_H

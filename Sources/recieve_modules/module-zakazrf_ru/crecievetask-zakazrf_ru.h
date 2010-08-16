#ifndef CRECIEVETASK_ZAKAZRF_RU_H
#define CRECIEVETASK_ZAKAZRF_RU_H

#include <QObject>
#include <QList>
#include <QMutex>
#include <QVariantList>
#include <QtPlugin>

#include <crecievetask.h>
#include <crecivethread.h>

class CRecieveTask_zakazrf_ru : public QObject, public CRecieveTask
{
    Q_OBJECT
    Q_INTERFACES(CRecieveTask)
public:
    CRecieveTask_zakazrf_ru();
    virtual bool init(int maxThreads, const siterules_ti& rule);
    virtual CRecieveTaskSignaller* signaller();
    virtual bool run();

private:
    QUrl createFullUrlFromRule(QUrl url,QVariant rule);

    QList<CReciveThread*> m_threads;
    QUrl m_url;
    QVariantList m_rules;
    int m_maxThreads;
    CRecieveTaskSignaller *m_signaller;
public slots:
    virtual void onDataReady(int i, QByteArray data);
    virtual void onThreadFinished();
};

#endif // CRECIEVETASK_ZAKAZRF_RU_H

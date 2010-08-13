#ifndef CRECIEVETASK_H
#define CRECIEVETASK_H

#include <QObject>
#include <QHttp>
#include <QThread>
#include <QList>
#include <QMutex>

#include "loadbalancer.h"
#include "cdatabasehandler.h"
#include "constants.h"

class CReciveThread : public QThread
{
    Q_OBJECT
public:
    CReciveThread(QUrl url, int id, QObject* parent = 0);
    ~CReciveThread();

    void setId(int id) {m_threadId=id;}
    int id() {return m_threadId;}
signals:
    void dataReady(int id, QByteArray data);

protected:
    virtual void run();

    QUrl m_url;
    QHttp m_http;
    int m_threadId;
    int m_httpId;
    QByteArray recivedData;

protected slots:
    void onRecieveComplete(int id, bool error);
};

class CRecieveTask : public QObject
{
    Q_OBJECT
public:
    CRecieveTask(CDataBaseHandler*, const siterules_ti& rule);
    bool run();
    void destroy();

    CDataBaseHandler* database() {return m_pDataBase;};

protected:
    virtual QUrl createFullUrlFromRule(QUrl url,QVariant rule);

    LoadBalancer m_taskBalancer;
    CDataBaseHandler* m_pDataBase;
    QList<CReciveThread*> m_threads;
    QUrl m_url;
    QVariantList m_rules;

public slots:
    void onDataReady(int i, QByteArray data);
    void onThreadFinished();
signals:
    void finished(CRecieveTask*);
};

#endif // CRECIEVETASK_H

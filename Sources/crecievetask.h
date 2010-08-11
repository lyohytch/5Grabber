#ifndef CRECIEVETASK_H
#define CRECIEVETASK_H

#include <QObject>
#include <QHttp>

#include "loadbalancer.h"
#include "cdatabasehandler.h"
#include "constants.h"

class CRecieveTask : public QObject
{
    Q_OBJECT
public:
    CRecieveTask(CDataBaseHandler*, const siterules_ti& rule);
    bool run();
    void destroy();

    CDataBaseHandler* database() {return m_pDataBase;};
protected:
    LoadBalancer m_taskBalancer;
    CDataBaseHandler* m_pDataBase;
    QUrl m_url;
    QVariantList m_rules;
    QHttp m_http;
    int m_id;
signals:
    void finished(CRecieveTask*);
public slots:
    void onRecieveComplete(int id, bool error);
};

#endif // CRECIEVETASK_H

#ifndef CRECIEVETASK_H
#define CRECIEVETASK_H

#include "loadbalancer.h"
#include "cdatabasehandler.h"
#include <QObject>
#include "constants.h"

class CRecieveTask : public QObject
{
    Q_OBJECT
public:
    CRecieveTask(CDataBaseHandler*, const siterules_ti& rule);
    bool run()
    {
        emit finished(this);
        return true;
    };
    void destroy(){};

    CDataBaseHandler* database() {return m_pDataBase;};
protected:
    LoadBalancer m_taskBalancer;
    CDataBaseHandler* m_pDataBase;
signals:
    void finished(CRecieveTask*);
};

#endif // CRECIEVETASK_H

#ifndef CRECIEVETASK_TEST_H
#define CRECIEVETASK_TEST_H

#include <crecievetask.h>

#include <QObject>
#include <QtPlugin>

class CRecieveTask_test : public QObject, public CRecieveTask
{
    Q_OBJECT
    Q_INTERFACES(CRecieveTask)
public:
    CRecieveTask_test();
    virtual bool init(int maxThreads, const siterules_ti& rule);
    virtual CRecieveTaskSignaller* signaller();
    virtual bool run();
private:
    CRecieveTaskSignaller *m_signaller;
};

#endif // CRECIEVETASK_TEST_H

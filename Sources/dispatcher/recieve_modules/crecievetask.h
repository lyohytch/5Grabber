#ifndef CRECIEVETASK_H
#define CRECIEVETASK_H

#include <QObject>
#include <QtPlugin>

#include <constants.h>
#include <crecievetasksignaller.h>

class CRecieveTask
{
public:
    virtual bool init(int maxThreads, const siterules_ti& rule)=0;
    virtual CRecieveTaskSignaller* signaller() = 0;
    virtual bool run()=0;
};

Q_DECLARE_INTERFACE (CRecieveTask, "recievetask.grabber.interface/1.0");

#endif // CRECIEVETASK_H

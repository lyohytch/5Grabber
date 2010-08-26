#ifndef CPARSETHREAD_H
#define CPARSETHREAD_H

//#include <QObject>
#include <QThread>

#include "tp_task.h"

class CParseThread : public QThread
{
    Q_OBJECT
public:
    CParseThread();
    void run();

signals:

};

#endif // CPARSETHREAD_H

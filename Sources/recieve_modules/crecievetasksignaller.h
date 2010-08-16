#ifndef CRECIEVETASKSIGNALLER_H
#define CRECIEVETASKSIGNALLER_H

#include <QObject>
#include <QDebug>
class CRecieveTask;

class CRecieveTaskSignaller : public QObject
{
    Q_OBJECT
public:
    CRecieveTaskSignaller(){}
    ~CRecieveTaskSignaller(){}
public slots:
    void onRecieveFinished(CRecieveTask* task)
    {
        emit finished(task);
    }

signals:
    void finished(CRecieveTask*);
};

#endif //CRECIEVETASKSIGNALLER_H


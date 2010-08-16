#ifndef CRECIEVETASKSIGNALLER_H
#define CRECIEVETASKSIGNALLER_H

#include <QObject>
#include <QDebug>
#include <datastructure/cdatastructure.h>

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

    void onDataReady(CDataStructure* data)
    {
        emit dataReady(data);
    }

signals:
    void finished(CRecieveTask*);
    void dataReady(CDataStructure*);
};

#endif //CRECIEVETASKSIGNALLER_H


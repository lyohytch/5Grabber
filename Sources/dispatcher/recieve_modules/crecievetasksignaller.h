#ifndef CRECIEVETASKSIGNALLER_H
#define CRECIEVETASKSIGNALLER_H

#include <QObject>
#include <QDebug>
#include <cdatastructure.h>

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

    void onDataParsed(QUrl url)
    {
        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
        emit dataParsed(url);
    }

signals:
    void finished(CRecieveTask*);
    void dataReady(CDataStructure*);
    void dataParsed(QUrl);
};

#endif //CRECIEVETASKSIGNALLER_H


#ifndef CPARSEDISPATCHER_H
#define CPARSEDISPATCHER_H

#include <QObject>
#include <QList>
#include <QMap>

#include "cdatastructure.h"
#include "cparsethread.h"

class CParseDispatcher : public QObject
{
    Q_OBJECT
public:
    CParseDispatcher();
    ~CParseDispatcher();
    //QList<CDataStructure*> queue() {return m_queue;}
    void addToQueue(CDataStructure* newTask);
    void onAddedToQueue();

signals:
    void queueMemeberParsed(CDataStructure* p_data);

public slots:
//    void onAddedToQueue();
    void onParseTaskFinished();

private:
    int m_maxThreads;
    QMap <CParseThread* , CDataStructure*> m_threads;
//    QList<CParseThread*> m_threads;
    QList<CDataStructure*> m_queue;
    QList<CDataStructure*> m_activeQueue;
};

#endif // CPARSEDISPATCHER_H

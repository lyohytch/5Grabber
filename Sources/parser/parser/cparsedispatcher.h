#ifndef CPARSEDISPATCHER_H
#define CPARSEDISPATCHER_H

#include <QObject>
#include <QList>

#include "cdatastructure.h"

class CParseDispatcher : public QObject
{
    Q_OBJECT
public:
    CParseDispatcher();
    ~CParseDispatcher();
    QList<CDataStructure*> queue() {return m_queue;}

signals:
    void queueMemeberParsed(CDataStructure* p_data);

public slots:
    void onAddedToQueue();
    void onParseTaskFinished(CDataStructure* p_data);

private:
    int m_maxThreads;
    QList<CDataStructure*> m_queue;
    QList<CDataStructure*> m_activeQueue;
};

#endif // CPARSEDISPATCHER_H

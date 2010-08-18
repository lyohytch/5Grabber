#ifndef CRECIEVETHREAD_H
#define CRECIEVETHREAD_H

#include <QHttp>
#include <QThread>
#include <QMutex>
#include <QUrl>

#include <cdatastructure.h>

class CReciveThread : public QThread
{
    Q_OBJECT
public:
    CReciveThread(QUrl url, int id, QObject* parent = 0);
    ~CReciveThread();

    void setId(int id) {m_threadId=id;}
    int id() {return m_threadId;}

    bool setDataStructure(CDataStructure* data)
    {
        if(!data)
        {
            return false;
        }
        m_data=data;
        return true;
    };

    CDataStructure* data() {return m_data;}

signals:
//    void dataReady(int id, QByteArray data);
    void dataReady(int id);

protected:
    virtual void run();

    QUrl m_url;
    QHttp m_http;
    int m_threadId;
    int m_httpId;

    CDataStructure* m_data;

protected slots:
    void onRecieveComplete(int id, bool error);
    void onError();
};
#endif //CRECIEVETHREAD_H

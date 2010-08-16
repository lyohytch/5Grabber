#ifndef CRECIEVETHREAD_H
#define CRECIEVETHREAD_H

#include <QHttp>
#include <QThread>
#include <QMutex>
#include <QUrl>

class CReciveThread : public QThread
{
    Q_OBJECT
public:
    CReciveThread(QUrl url, int id, QObject* parent = 0);
    ~CReciveThread();

    void setId(int id) {m_threadId=id;}
    int id() {return m_threadId;}

signals:
    void dataReady(int id, QByteArray data);

protected:
    virtual void run();

    QUrl m_url;
    QHttp m_http;
    int m_threadId;
    int m_httpId;
    QByteArray recivedData;

protected slots:
    void onRecieveComplete(int id, bool error);
};
#endif //CRECIEVETHREAD_H

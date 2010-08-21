#include "tparsereceiver.h"
#include "tp_task.h"
#include  <QPluginLoader>
#include  <QMap>
#include  <QVariant>
#include  <QString>
#include  <QDebug>


bool TParseReceiver::parse(CDataStructure* _data, const QByteArray& _url)
{
    if(NULL == _data)
    {
        qDebug() << "parse: null pointer";
        return FALSE;
    }

    qDebug()<<_data->childscCount();
    qDebug()<<_data->url();
    TParseThread *thread = new TParseThread(m_id,_data,_url);
//    m_threads.insert(m_id,thread);
    m_threads.push_back(thread);
    m_id++;
    connect(thread, SIGNAL(finished()), this, SLOT(onThreadFinished()));
    thread->start();
    return TRUE;
}

void TParseReceiver::onThreadFinished()
{
    for(int i=0; i<m_threads.count(); i++)
    {
        if(!m_threads.value(i)->isFinished())
        {
            continue;
        }

        QVariant vdata(m_threads.value(i)->result());
        qDebug() << m_threads.value(i)->result();

        //write(vdata);!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!отдать данные Диме
        emit finished(m_threads.value(i)->error(), m_threads.value(i)->data()->root()->url().toString().toUtf8());
        delete m_threads.value(i);
        m_threads.removeAt(i);
    }
}




TParseThread::TParseThread(int _id, CDataStructure* _data, const QByteArray& _url)
{
    m_id = _id;
    m_url = _url;
    m_data = _data;


    if(m_url == URL1)
    {
        m_path = PATH_MODULES + SO1;
    }
    else
    {
        m_path = "";
    }
}

void TParseThread::run()
{
    QPluginLoader task(m_path);
    if( FALSE == task.load() )
    {
        qDebug() << "run: load error"<<task.errorString();
        return;
    }

    TP_Task* plugin=qobject_cast<TP_Task *>(task.instance());
    m_result = plugin->parse(m_data);
    m_error=NO_ERROR;

}

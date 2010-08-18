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

    TParseThread *thread = new TParseThread(m_id,_data,_url);
    m_threads.insert(m_id,thread);
    m_id++;
    connect(thread, SIGNAL(end_parsing_signal(int,int,const QByteArray&,const QMap<QString,QVariant>&)), this, SLOT(end_parsing(int,int,const QByteArray&,const QMap<QString,QVariant>&)));
    thread->start();
    return TRUE;
}

void TParseReceiver::end_parsing(int _id,int _error,const QByteArray& _url,const QMap<QString,QVariant>& _data)
{
    m_threads.value(_id)->exit();
    delete m_threads.value(_id);
    m_threads.remove(_id);

    QVariant vdata(_data);
    qDebug() << _data;

    //write(vdata);!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!отдать данные Диме
    emit finished(_error,_url);
}






TParseThread::TParseThread(int _id, CDataStructure* _data, const QByteArray& _url)
{
    id = _id;
    url = _url;
    data = _data;


    if(url == URL1)
    {
        path = PATH_MODULES + SO1;
    }
    else
    {
        path = "";
    }
}

void TParseThread::run()
{
    QPluginLoader task(path);
    if( FALSE == task.load() )
    {
        qDebug() << "run: load error";
        emit end_parsing_signal(id,DATA_NOT_FOUND,url,QMap<QString,QVariant>());
        return;
    }

    TP_Task* plugin=qobject_cast<TP_Task *>(task.instance());
    QMap<QString,QVariant> result = plugin->parse(data);
    emit end_parsing_signal(id,NO_ERROR,url,result);
}





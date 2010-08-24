#include "tparsereceiver.h"

#include <QPluginLoader>
#include <QMap>
#include <QVariant>
#include <QString>
#include <QDebug>

bool TParseReceiver::init()
{
    m_dispatcher = new CParseDispatcher();
    if (m_dispatcher != NULL)
    {
        connect(this, SIGNAL(addToQueue()), m_dispatcher, SLOT(onAddedToQueue()));
        connect(m_dispatcher, SIGNAL(queueMemeberParsed(CDataStructure*)), this, SLOT(onQueueMemberParsed(CDataStructure*)));
        return TRUE;
    }
    return FALSE;
}

bool TParseReceiver::parse(CDataStructure* _data, const QByteArray& _url)
{
    Q_UNUSED(_url);
    if(NULL == _data)
    {
        qDebug() << "parse: null pointer";
        return FALSE;
    }

    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"Childs count "<< _data->childscCount();
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"URL" << _data->url();

    m_dispatcher->queue().push_back(_data);
    emit addToQueue();

//    QPluginLoader loader(PATH_MODULES + PARSE_MODULE);
//    if(!loader.load())
//    {
//        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<loader.errorString();
//        return FALSE;
//    }
//    TP_Task* task = qobject_cast<TP_Task *>(loader.instance());
//    task->init(1, m_data);
//    //loader.unload()
//
//    m_parseTasksList.append(task);
//    connect(task->signaller(), SIGNAL(finishedParse()), this, SLOT(onParseFinished()));
////    connect(task->signaller(), SIGNAL(dataReady(CDataStructure*)), this, SLOT(onRecieveDataReady(CDataStructure*)));
//    task->run();
//
//    QPluginLoader loaderDoc(PATH_MODULES + PARSE_DOC_MODULE);
//    if(!loaderDoc.load())
//    {
//        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<loaderDoc.errorString();
//        return FALSE;
//    }
//    TP_Task* taskDoc = qobject_cast<TP_Task *>(loaderDoc.instance());
//    taskDoc->init(1, m_data);
//    //loader.unload()
//
//    m_parseDocTasksList.append(taskDoc);
//    connect(taskDoc->signaller(), SIGNAL(finishedParse()), this, SLOT(onParseDocFinished()));
////    connect(task->signaller(), SIGNAL(dataReady(CDataStructure*)), this, SLOT(onRecieveDataReady(CDataStructure*)));
//    taskDoc->run();

    return TRUE;
}

//void TParseReceiver::onParseFinished()
//{
//    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
//    m_done = TRUE;
//    if (m_done && m_doneDocs)
//    {
////        emit finishedParse(0, ba);
//    }
//}
//
//void TParseReceiver::onParseDocFinished()
//{
//    //qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
//    m_doneDocs = TRUE;
//    if (m_done && m_doneDocs)
//    {
//        m_doneDocs = FALSE;
//        m_done = FALSE;
//        const QByteArray ba = m_data->url().toString().toUtf8();
//        emit finishedParse(0, ba);
//    }
//}

void TParseReceiver::onQueueMemberParsed(CDataStructure *p_data)
{
    const QByteArray ba = p_data->root()->url().toString().toUtf8();
    emit finishedParse(0, ba);
}

//void TParseReceiver::onTimeOut()
//{
//    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
//    emit finishedParse(0, QByteArray());
//}

//void TParseReceiver::onThreadFinished()
//{
//    for(int i=0; i<m_threads.count(); i++)
//    {
//        if(!m_threads.value(i)->isFinished())
//        {
//            continue;
//        }
//
//        QVariant vdata(m_threads.value(i)->result());
//        qDebug() << m_threads.value(i)->result();
//
//        //write(vdata);!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!отдать данные Диме
//        emit finishedParse(m_threads.value(i)->error(), m_threads.value(i)->data()->root()->url().toString().toUtf8());
//        delete m_threads.value(i);
//        m_threads.removeAt(i);
//    }
//}

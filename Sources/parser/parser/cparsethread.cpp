#include "cparsethread.h"

#include <QDebug>
#include <QPluginLoader>

CParseThread::CParseThread(CDataStructure *data, DBmanager *db): m_data(data), m_db(db)
{
    // nothing
}

void CParseThread::run()
{
    qDebug() << "============ HELLO from thread " << this->currentThreadId();
    qDebug() << "====================== URL" << m_data->root()->url().host();
    if (m_data->root()->url().host() == "zakazrf.ru")
    {

        qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
        qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
        qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
        qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";

//        QPluginLoader loader(PATH_MODULES + PARSE_MODULE);
//        if(!loader.load())
//        {
//            qDebug() << loader.errorString();
//        }
//        TP_Task* task = qobject_cast<TP_Task *>(loader.instance());
//        task->init(1, m_data, m_db);
//        //loader.unload()
//
//        //m_parseTasksList.append(task);
//        //connect(task->signaller(), SIGNAL(finishedParse()), this, SLOT(onParseFinished()));
//        //    connect(task->signaller(), SIGNAL(dataReady(CDataStructure*)), this, SLOT(onRecieveDataReady(CDataStructure*)));
//        task->run();
//
//        QPluginLoader loaderDoc(PATH_MODULES + PARSE_DOC_MODULE);
//        if(!loaderDoc.load())
//        {
//            qDebug() << loaderDoc.errorString();
//        }
//        TP_Task* taskDoc = qobject_cast<TP_Task *>(loaderDoc.instance());
//        taskDoc->init(1, m_data, m_db);
//        //loader.unload()
//
//        //m_parseDocTasksList.append(taskDoc);
//        //connect(taskDoc->signaller(), SIGNAL(finishedParse()), this, SLOT(onParseDocFinished()));
//        //    connect(task->signaller(), SIGNAL(dataReady(CDataStructure*)), this, SLOT(onRecieveDataReady(CDataStructure*)));
//        taskDoc->run();
    }
    else if (m_data->root()->url().host() == "sberbank-ast.ru")
    {
        qDebug() << "################################################################";
        qDebug() << "################################################################";
        qDebug() << "################################################################";
        qDebug() << "################################################################";
    }
}

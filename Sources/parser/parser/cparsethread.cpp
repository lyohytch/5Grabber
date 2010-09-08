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
    if (m_data->root()->url().host() == "zakazrf.ru")
    {
        QPluginLoader loader(PATH_MODULES + PARSE_MODULE_ZAKAZRF);
        if(!loader.load())
        {
            qDebug() << loader.errorString();
        }
        else
        {
            TP_Task* task = qobject_cast<TP_Task *>(loader.instance());
            task->init(1, m_data, m_db);
            //loader.unload()

            //m_parseTasksList.append(task);
            //connect(task->signaller(), SIGNAL(finishedParse()), this, SLOT(onParseFinished()));
            //    connect(task->signaller(), SIGNAL(dataReady(CDataStructure*)), this, SLOT(onRecieveDataReady(CDataStructure*)));
            task->run();
        }

        QPluginLoader loaderDoc(PATH_MODULES + PARSE_MODULE_ZAKAZRF_DOC);
        if(!loaderDoc.load())
        {
            qDebug() << loaderDoc.errorString();
            return;
        }
        else
        {
            TP_Task* taskDoc = qobject_cast<TP_Task *>(loaderDoc.instance());
            taskDoc->init(1, m_data, m_db);
            //loader.unload()

            //m_parseDocTasksList.append(taskDoc);
            //connect(taskDoc->signaller(), SIGNAL(finishedParse()), this, SLOT(onParseDocFinished()));
            //    connect(task->signaller(), SIGNAL(dataReady(CDataStructure*)), this, SLOT(onRecieveDataReady(CDataStructure*)));
            taskDoc->run();
        }
    }
    else if (m_data->root()->url().host() == "sberbank-ast.ru")
    {
        QPluginLoader loader(PATH_MODULES + PARSE_MODULE_SBER);
        if(!loader.load())
        {
            qDebug() << loader.errorString();
        }
        else
        {
            qDebug() << "################################################################";
            qDebug() << "################################################################";
            TP_Task* task = qobject_cast<TP_Task *>(loader.instance());
            task->init(1, m_data, m_db);
            //loader.unload()

            //m_parseTasksList.append(task);
            //connect(task->signaller(), SIGNAL(finishedParse()), this, SLOT(onParseFinished()));
            //    connect(task->signaller(), SIGNAL(dataReady(CDataStructure*)), this, SLOT(onRecieveDataReady(CDataStructure*)));
            task->run();
        }
    }
}

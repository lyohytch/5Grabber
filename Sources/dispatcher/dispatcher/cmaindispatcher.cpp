#include "cmaindispatcher.h"
#include <QDebug>
#include <QPluginLoader>
#include <QDateTime>

CMainDispatcher::CMainDispatcher(QObject *parent) :
    QObject(parent)
{
}

CMainDispatcher::~CMainDispatcher()
{
    deinit();
}

bool CMainDispatcher::init()
{
    return init(cConfigUrl.toString());
}

bool CMainDispatcher::init(const QString &configUrl)
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
    QUrl tmpConfigUrl(configUrl);

    if(tmpConfigUrl!=cConfigUrl && !checkUrl(tmpConfigUrl))
    {
        tmpConfigUrl=cConfigUrl;
    }

    if(!g_config.load(tmpConfigUrl))
    {
        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"Failed";
        return false;
    }

    m_sites = g_config.loadSites();
    if(m_sites.isEmpty())
    {
        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"Failed";
        return false;
    }

    if(!connectActions())
    {
        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"Failed";
        return false;
    }

    if(!m_parser.init())
    {
        return false;
    }

    if(!connect(&m_parser,SIGNAL(parceFinished(int,QUrl)),this,SLOT(onParceFinished(int,QUrl))))
    {
        return false;
    }

    startRecieveTasks();
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"Succed";
    return true;
}

void CMainDispatcher::deinit()
{

//    for(int i=0;i<m_activeTasksList.count();i++)
//    {
//        delete m_activeTasksList.at(i);
//    }

    m_activeTasksList.clear();

    m_sites.clear();
}

void CMainDispatcher::startRecieveTasks()
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<m_sites.count();
//    CRecieveTask* tmpTask;
    siterules_ti Iter(m_sites);

    while(Iter.hasNext())
    {
        Iter.next();

        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<Iter.key().host().replace(".","_");

        qDebug()<<"RUN_TIME_PATH"<<RUN_TIME_PATH<<"\nlibpath :"<<QString("./modules/recieve/libmodule-%1.so").arg(Iter.key().host().replace(".","_"));
        QPluginLoader loader(QString("./modules/recieve/libmodule-%1.so").arg(Iter.key().host().replace(".","_")));
        if(!loader.load())
        {
            qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<loader.errorString();
            continue;
        }

        CRecieveTask* task=qobject_cast<CRecieveTask *>(loader.instance());
        task->init(2, Iter);
//        loader.unload();

        m_activeTasksList.append(task);
        connect(task->signaller(), SIGNAL(finished(CRecieveTask*)), this, SLOT(onRecieveTaskFinished(CRecieveTask*)));
        connect(task->signaller(), SIGNAL(dataReady(CDataStructure*)), this, SLOT(onRecieveDataReady(CDataStructure*)));
    }

    for(int i=0;i<m_activeTasksList.count();i++)
    {
        m_activeTasksList.at(i)->run();
    }
}

void CMainDispatcher::onRecieveTaskFinished(CRecieveTask *task)
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"Task Finished";

    m_activeTasksList.removeOne(task);

    if(m_activeTasksList.isEmpty())
    {
        emit done();
    }
}


void CMainDispatcher :: onParceFinished(int error, QUrl url)
{
    qCritical()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<": url"<<url.host();
    qCritical()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<< "##############################" << QDateTime::currentDateTime();
    CRecieveTask* task=NULL;
    for(int i=0; i<m_activeTasksList.count(); i++)
    {
        if(m_activeTasksList.at(i)->taskHost()==url.host())
        {
            task=m_activeTasksList.value(i);
        }
    }

    if(!task)
    {
        qCritical()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<": task for host "<<url.host()<<" not found";
        return;
    }

    task->signaller()->onDataParsed(url);
}

void CMainDispatcher::onRecieveDataReady(CDataStructure* data)
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"Data structure is ready:"<<data->url();
    QUrl url;
    url.setHost(data->url().host());
    url.setScheme(data->url().scheme());
    m_parser.startParsing(data, url);
}


void CMainDispatcher::onDone()
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<"Multi launch mode is switched off for preview";
//    m_startTasksTimer.setSingleShot(true);
//    m_startTasksTimer.start(20000);
}

bool CMainDispatcher::connectActions()
{
    bool m_connected=true;
    m_connected&=connect(&m_startTasksTimer, SIGNAL(timeout()), this, SLOT(startRecieveTasks()));
    m_connected&=connect(this, SIGNAL(done()), this, SLOT(onDone()));
    return m_connected;
}

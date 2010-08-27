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
    CConfigHandler::flush();
}

bool CMainDispatcher::init()
{
    return init(cConfigUrl);
}

bool CMainDispatcher::init(const QString &configUrl)
{
    qDebug()<<"======================Start Grabber====================";
    qDebug()<<"RUN_TIME_PATH"<<RUN_TIME_PATH;;
    qDebug()<<"Time"<<QDateTime::currentDateTime().toTime_t();
#ifdef TIME_STAMPS
    gSummaryWorkTime=QDateTime::currentDateTime().toTime_t();
#endif
    if(!CConfigHandler::load(configUrl))
    {
        qDebug()<<"Config initialization error";
        return false;
    }

    if(!CConfigHandler::loadSites(m_sites))
    {
        qDebug()<<"Failed to load sites";
        return false;
    }

    if(m_sites.isEmpty())
    {
        qDebug()<<"No sites to download";
        return false;
    }

    if(!connectActions())
    {
        qDebug()<<"Failed";
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
    qDebug()<<"Succed";
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
    qDebug()<<m_sites.count();
//    CRecieveTask* tmpTask;
    siterules_ti Iter(m_sites);
    qDebug()<<m_sites;

    while(Iter.hasNext())
    {
        Iter.next();

        qDebug()<<Iter.key().host().replace(".","_");
        QPluginLoader loader(QString("./modules/recieve/libmodule-%1.so").arg(Iter.key().host().replace(".","_")));
        if(!loader.load())
        {
            qWarning()<<"Library: "<< QString("./modules/recieve/libmodule-%1.so").arg(Iter.key().host().replace(".","_"))<<loader.errorString();
            continue;
        }

        CRecieveTask* task=qobject_cast<CRecieveTask *>(loader.instance());
        task->init(getConfigurationValue("main/max_threads",QVariant(5)).toInt(), Iter);
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
    qDebug()<<"Task Finished";

    m_activeTasksList.removeOne(task);

    if(m_activeTasksList.isEmpty())
    {
        emit done();
    }
}


void CMainDispatcher :: onParceFinished(int error, QUrl url)
{
    qDebug()<<"url:"<<url.host()<<"error:"<<error;
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
        qCritical()"task for host "<<url.host()<<" not found";
        return;
    }

    task->signaller()->onDataParsed(url);
}

void CMainDispatcher::onRecieveDataReady(CDataStructure* data)
{
    qDebug()<<"Data structure is ready:"<<data->url();
    QUrl url;
    url.setHost(data->url().host());
    url.setScheme(data->url().scheme());
    m_parser.startParsing(data, url);
}


void CMainDispatcher::onDone()
{
    qDebug()<<"Multi launch mode is switched off for preview";
#ifdef TIME_STAMPS
    gSummaryWorkTime=QDateTime::currentDateTime().toTime_t()-gSummaryWorkTime;
    qDebug()<<"######################## Summary work time: "<<gSummaryWorkTime<<" ########################";
#endif
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

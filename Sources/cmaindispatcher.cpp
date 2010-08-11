#include "cmaindispatcher.h"
#include <QDebug>

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

    if(!m_config.load(tmpConfigUrl))
    {
        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"Failed";
        return false;
    }

    m_sites = m_config.loadSites();
    if(m_sites.isEmpty())
    {
        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"Failed";
        return false;
    }

    if(!prepareDataBases())
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

    startRecieveTasks();
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"Succed";
    return true;
}

void CMainDispatcher::deinit()
{
    QMapIterator<QUrl, CDataBaseHandler*> Iter(m_preparedDataBases);
    while(Iter.hasNext())
    {
        Iter.next();
        delete(Iter.value());
    }
    m_preparedDataBases.clear();

    for(int i=0;i<m_activeTasksList.count();i++)
    {
        delete m_activeTasksList.at(i);
    }

    m_activeTasksList.clear();

    m_sites.clear();
}

bool CMainDispatcher::prepareDataBases()
{
    CDataBaseHandler* tmpDatabase;
    siterules_ti Iter(m_sites);

    while(Iter.hasNext())
    {
        Iter.next();
        tmpDatabase=new CDataBaseHandler();
        if(!tmpDatabase->open(Iter.key()))
        {
            qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"Can't create database for URL:"<<Iter.key();
            delete tmpDatabase;
            continue;
        }
        m_preparedDataBases.insert(Iter.key(), tmpDatabase);

    }
    return true;
}

void CMainDispatcher::startRecieveTasks()
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
    CRecieveTask* tmpTask;
    siterules_ti Iter(m_sites);

    if(m_preparedDataBases.isEmpty())
    {
        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"DataBase List corrupted";
        return;
    }

    while(Iter.hasNext())
    {
        Iter.next();
        if(!m_preparedDataBases.contains(Iter.key()))
        {
            qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"DataBase List corrupted";
            return;
        }
        tmpTask = new CRecieveTask(m_preparedDataBases.value(Iter.key()), Iter);

        m_activeTasksList.append(tmpTask);
        connect(tmpTask, SIGNAL(finished(CRecieveTask*)), this, SLOT(onRecieveTaskFinished(CRecieveTask*)));
    }

    for(int i=0;i<m_activeTasksList.count();i++)
    {
        m_activeTasksList.at(i)->run();
    }
}

void CMainDispatcher::onRecieveTaskFinished(CRecieveTask *task)
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<<"Task Finished";
    CDataBaseHandler* tmpDB;
    tmpDB=task->database();
    task->destroy();
    m_activeTasksList.removeOne(task);
    delete task;

    m_parser.startParsing(tmpDB);
    if(m_activeTasksList.isEmpty())
    {
        emit done();
    }
}

void CMainDispatcher::onDone()
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
    m_startTasksTimer.setSingleShot(true);
    m_startTasksTimer.start(5000);
}

bool CMainDispatcher::connectActions()
{
    bool m_connected=true;
    m_connected&=connect(&m_startTasksTimer, SIGNAL(timeout()), this, SLOT(startRecieveTasks()));
    m_connected&=connect(this, SIGNAL(done()), this, SLOT(onDone()));
    return m_connected;
}
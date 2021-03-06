#ifndef CMAINDISPATCHER_H
#define CMAINDISPATCHER_H

#include <QObject>
#include <QUrl>
#include <QString>
#include <QTimer>
#include <QList>
#include <QPluginLoader>

#include <constants.h>
#include <crecievetask.h>
#include <cconfighandler.h>
#include <cdatastructure.h>
#include "cparserhandler.h"

static unsigned int gSummaryWorkTime=0;

class CMainDispatcher : public QObject
{
Q_OBJECT
public:
    explicit CMainDispatcher(QObject *parent = 0);
    ~CMainDispatcher();
    bool init();
    bool init(const QString& configUrl);
    void deinit();

protected:
//    void addTask(CRecieveTask* task);
    bool connectActions();

    QTimer m_startTasksTimer;
    QMap<CRecieveTask*, QPluginLoader*> m_activeTasksList;

    siterules_t m_sites;
    CParserHandler m_parser;
signals:
    void done();
public slots:
    void onRecieveTaskFinished(CRecieveTask *task);
    void onRecieveDataReady(CDataStructure* data);
    void onParceFinished(int error, QUrl url);

protected slots:
    void startRecieveTasks();
    void onDone();
};

#endif // CMAINDISPATCHER_H

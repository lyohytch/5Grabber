#ifndef CMAINDISPATCHER_H
#define CMAINDISPATCHER_H

#include <QObject>
#include <QTimer>
#include <QList>

#include <crecievetask.h>

#include "cconfighandler.h"
#include "cdatabasehandler.h"
#include "constants.h"
#include "cparserhandler.h"

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
    bool checkUrl(const QUrl& configUrl) {return true;}
    bool prepareDataBases();
    bool connectActions();

    QTimer m_startTasksTimer;
    QList<CRecieveTask*> m_activeTasksList;
    QMap<QUrl, CDataBaseHandler*> m_preparedDataBases;

    siterules_t m_sites;
    CParserHandler m_parser;
signals:
    void done();
public slots:
    void onRecieveTaskFinished(CRecieveTask *task);
    void onRecieveDataReady(CDataStructure* data);
protected slots:
    void startRecieveTasks();
    void onDone();
};

#endif // CMAINDISPATCHER_H

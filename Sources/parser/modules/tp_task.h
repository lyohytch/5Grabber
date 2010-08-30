#ifndef TP_TASK_H
#define TP_TASK_H

#include <QObject>
#include <QtPlugin>
#include <QMap>
#include <QString>
#include <QVariant>

#include <cdatastructure.h>
#include <cparsesignaller.h>
#include "dbmanager.h"

class TP_Task
{
public:
    virtual bool init(int maxThreads, CDataStructure* data, DBmanager* db) = 0;
    //virtual QMap<QString,QVariant> parse(CDataStructure* _data) = 0;
    virtual bool run() = 0;
    virtual CParseSignaller* signaller() = 0;
};

Q_DECLARE_INTERFACE (TP_Task, "tp_task.grabber.interface/1.0");

#endif

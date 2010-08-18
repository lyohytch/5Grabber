#ifndef TP_TASK_H
#define TP_TASK_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QtPlugin>
#include <cdatastructure.h>

class TP_Task
{
public:
    virtual QMap<QString,QVariant> parse(CDataStructure* _data)=0;
};

Q_DECLARE_INTERFACE (TP_Task, "tp_task.grabber.interface/1.0");

#endif

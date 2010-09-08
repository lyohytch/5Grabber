#ifndef CPARSETHREAD_H
#define CPARSETHREAD_H

//#include <QObject>
#include <QThread>

#include "tp_task.h"

//======================================================
//modules
const QString PATH_MODULES = "./modules/parse/";
const QString URL1 = "http://zakazrf.ru";
const QString PARSE_MODULE_ZAKAZRF = "libm_zakazrf.so";
const QString PARSE_MODULE_SBER = "libm_sberbank.so";
const QString PARSE_MODULE_ZAKAZRF_DOC = "libm_zakazrf_doc.so";
//======================================================

class CParseThread : public QThread
{
    Q_OBJECT
public:
    CParseThread(CDataStructure* data, DBmanager* db);
    void run();

signals:

private:
    CDataStructure* m_data;
    DBmanager* m_db;
};

#endif // CPARSETHREAD_H

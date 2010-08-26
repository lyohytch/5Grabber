#ifndef TPARSERECEIVER_H
#define TPARSERECEIVER_H

#include <cdatastructure.h>
#include "cparsedispatcher.h"

#include <QObject>
#include <QThread>

//======================================================
//modules
const QString PATH_MODULES = "./modules/parse/";
const QString URL1 = "http://zakazrf.ru";
const QString PARSE_MODULE = "libm_zakazrf.so";
const QString PARSE_DOC_MODULE = "libm_zakazrf_doc.so";

//code of errors
const QString error_code = "error_code";
const QString error_description = "error_description";
enum{ NO_ERROR = 0,
      DATA_NOT_FOUND };
//======================================================

class TParseReceiver: public QObject
{
    Q_OBJECT
    
public:
    TParseReceiver()/*:m_id(0), m_done(FALSE), m_doneDocs(FALSE)*/{}
    bool parse(CDataStructure* _data,const QByteArray& _url);
    bool init();

signals:
    void finishedParse(int _error, const QByteArray _url);
//    void addToQueue();

public slots:
//    void onParseFinished();
//    void onParseDocFinished();
    void onQueueMemberParsed(CDataStructure* p_data);
//    void onThreadFinished();
//    void onTimeOut();
//    void end_parsing(int _id, int _error,const QByteArray& _url,const QMap<QString,QVariant>& _data);

private:
//    int m_id;
    CParseDispatcher* m_dispatcher;
//    bool m_done;
//    bool m_doneDocs;
//    CDataStructure* m_data;
//    QMap<int,TParseThread*> m_threads;
    //QList<TParseThread*> m_threads;
//    QList<TP_Task*> m_parseTasksList;
//    QList<TP_Task*> m_parseDocTasksList;
};


#endif // TPARSERECEIVER_H

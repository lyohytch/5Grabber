#ifndef TPARSERECEIVER_H
#define TPARSERECEIVER_H
#include "cdatastructure.h"
#include <QObject>
#include <QThread>
#include <QMap>
#include <QVariant>


//======================================================
//modules
const QString PATH_MODULES = "../bin/";
const QString URL1 = "http://zakazrf.ru";
const QString SO1 = "libm_zakazrf.so";

//code of errors
const QString error_code = "error_code";
const QString error_description = "error_description";
enum{ NO_ERROR = 0,
      DATA_NOT_FOUND };
//======================================================


class TParseThread: public QThread
{
    Q_OBJECT
public:
    TParseThread(int _id,CDataStructure* _data, const QByteArray& _url);

signals:
    void end_parsing_signal(int _id,int _error,const QByteArray& _url,const QMap<QString,QVariant>& _data);

protected:
    virtual void run();

private:
    int id;
    QByteArray url;
    CDataStructure* data;

    QString path;
};

class TParseReceiver: public QObject
{
    Q_OBJECT
public:
    TParseReceiver():m_id(0){}
    bool parse(CDataStructure* _data,const QByteArray& _url);

signals:
    void finished(int _error,const QByteArray& _url);

private slots:
    void end_parsing(int _id, int _error,const QByteArray& _url,const QMap<QString,QVariant>& _data);

private:
    QMap<int,TParseThread*> m_threads;
    int m_id;
};


#endif // TPARSERECEIVER_H

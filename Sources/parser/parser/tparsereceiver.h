#ifndef TPARSERECEIVER_H
#define TPARSERECEIVER_H
#include "cdatastructure.h"
#include <QObject>
#include <QThread>
#include <QMap>
#include <QVariant>


//======================================================
//modules
const QString PATH_MODULES = "./modules/parse/";
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

//signals:
//    void end_parsing_signal(int _id,int _error,const QByteArray& _url,const QMap<QString,QVariant>& _data);
    QMap<QString,QVariant> result() {return m_result;}
    int id() const {return m_id;}
    QByteArray url() const {return m_url;}
    CDataStructure* data() const {return m_data;}
    int error(){return m_error;}

protected:
    virtual void run();

private:
    int m_id;
    QByteArray m_url;
    CDataStructure* m_data;
    QMap<QString,QVariant> m_result;
    QString m_path;
    int m_error;
};

class TParseReceiver: public QObject
{
    Q_OBJECT
public:
    TParseReceiver():m_id(0){}
    bool parse(CDataStructure* _data,const QByteArray& _url);

signals:
    void finished(int _error,const QByteArray _url);

public slots:
    void onThreadFinished();
//    void end_parsing(int _id, int _error,const QByteArray& _url,const QMap<QString,QVariant>& _data);

private:
//    QMap<int,TParseThread*> m_threads;
    QList<TParseThread*> m_threads;
    int m_id;
};


#endif // TPARSERECEIVER_H

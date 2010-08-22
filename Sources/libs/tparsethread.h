#ifndef TPARSETHREAD_H
#define TPARSETHREAD_H

#include "cdatastructure.h"

#include <QObject>
#include <QThread>
#include <QMap>
#include <QString>
#include <QVariant>

class TParseThread: public QThread
{
    Q_OBJECT
public:
    TParseThread();

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

#endif // TPARSETHREAD_H

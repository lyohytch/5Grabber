#ifndef TP_ZAKAZRF_DOC_H
#define TP_ZAKAZRF_DOC_H

#include <tp_task.h>
#include <cdatastructure.h>
#include "dbmanager.h"

#include <QObject>
#include <QStringList>
#include <QVariant>

class TP_zakazrf_doc : public QObject, public TP_Task
{
    Q_OBJECT
    Q_INTERFACES(TP_Task)

public:
    TP_zakazrf_doc();
    ~TP_zakazrf_doc();

    virtual bool init(int maxThreads, CDataStructure* data, DBmanager* db);
    virtual bool run();
    //virtual QMap<QString,QVariant> parse(CDataStructure* _data);
    virtual CParseSignaller* signaller();

private:
//    QMap<QString,QVariant> parse_html_auk(const QString& _html);
//    QMap<QString,QVariant> parse_html_lot(const QString& _html);
//    QString html_to_txt(const QString& _html);
    void docToXml(CDataStructure* p_data, int lot);
    QString findProviding(const QByteArray &source);
    qint64 m_threadCounter;
    int m_maxThreads;
    CDataStructure *m_data;
    CParseSignaller* m_signaller;
    DBmanager* m_db;
};

//class ParseDocXls : public QObject, public TP_Task
//{
//    Q_OBJECT
//    Q_INTERFACES(TP_Task)
//
//public:
//    explicit ParseDocXls(QObject *parent = 0);
//    virtual void readConfig(const QString &filename) {}
//    virtual QVariant ParseFile(const QString &filename) { return QVariant();}
//    QStringList catdocArgs;
//signals:
//
//public slots:
//
//};
//
//class ParseDoc : public ParseDocXls
//{
//Q_OBJECT
//public:
//    ParseDoc(QObject *parent = 0, const QString &filename = 0);
//    virtual void readConfig(const QString &filename);
//    virtual QVariant ParseFile(const QString &filename);
//signals:
//
//public slots:
//
//};
//
//class ParseXls : public ParseDocXls
//{
//Q_OBJECT
//public:
//    ParseXls(QObject *parent = 0, const QString &filename = 0);
//signals:
//
//public slots:
//
//};

#endif // TP_ZAKAZRF_DOC_H

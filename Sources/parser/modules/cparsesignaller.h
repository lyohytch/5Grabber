#ifndef CPARSESIGNALLER_H
#define CPARSESIGNALLER_H

#include <QObject>
#include <QDebug>

class TP_Task;

class CParseSignaller : public QObject
{
    Q_OBJECT
public:
    CParseSignaller(){}
    ~CParseSignaller(){}
public slots:
    void onParseFinished()
    {
        emit finishedParse();
    }
//    void onDataReady(CDataStructure* data)
//    {
//        emit dataReady(data);
//    }
//
//    void onDataParsed(QUrl url)
//    {
//        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;
//        emit dataParsed(url);
//    }

signals:
    void finishedParse();
};

#endif // CPARSESIGNALLER_H

#include <QDebug>
#include <QProcess>
#include <QByteArray>

#include "parsedocxls.h"

ParseDocXls::ParseDocXls(QObject *parent) :
    QObject(parent)
{
    readConfig(".configMS");
}

//ParseDoc
ParseDoc::ParseDoc(QObject *parent, const QString &filename):
     ParseDocXls(parent)
{
    qDebug()<<Q_FUNC_INFO<<" ( "<<__LINE__<<" ) : "<<filename;


}

void ParseDoc::readConfig(const QString &filename)
{
    qDebug()<<Q_FUNC_INFO<<" ( "<<__LINE__<<" )";
}

QVariant ParseDoc::ParseFile(const QString &filename)
{
    qDebug()<<Q_FUNC_INFO<<" ( "<<__LINE__<<" )";
    // FIXME
    QString fname = filename;
    fname.chop(4);
    fname += ".txt";
    catdocArgs << filename;
    QProcess *process = new QProcess(this);
    qDebug()<<Q_FUNC_INFO<<" ( "<<__LINE__<<" )"<<fname;
   // process->setStandardOutputFile(fname,QIODevice::ReadWrite);
    process->execute("/usr/local/bin/catdoc", catdocArgs);
    //process->waitForFinished();
    QByteArray arr = process->readAllStandardOutput();
    qDebug()<<Q_FUNC_INFO<<" (((( "<<__LINE__<<" )))) "<<arr;
    delete process;

    return QVariant();
}



// TODO implement it
//ParseXls
ParseXls::ParseXls(QObject *parent, const QString &filename):
     ParseDocXls(parent)
{

}

#include <QDebug>
#include <QProcess>
#include <QByteArray>
#include <QFile>

#include "parsedocxls.h"

ParseDocXls::ParseDocXls(QObject *parent) :
    QObject(parent)
{
}

//ParseDoc
ParseDoc::ParseDoc(QObject *parent, const QString &filename):
     ParseDocXls(parent)
{
     qDebug()<<Q_FUNC_INFO<<" ( "<<__LINE__<<" ) : "<<filename;
     targetFile = filename;
     readConfig(".configMS");

}

void ParseDoc::readConfig(const QString &/*filename*/)
{
    qDebug()<<Q_FUNC_INFO<<" ( "<<__LINE__<<" )";
    //TODO need to reading setting from file
}

QVariant ParseDoc::ParseFile()
{
    qDebug()<<Q_FUNC_INFO<<" ( "<<__LINE__<<" )";
    // FIXME
    //Constructing output filename
    QString outputFname = targetFile;
    outputFname.chop(4);
    outputFname += ".txt";
    //TODO rework it when I have more time
    //Converting doc to txt
    //Check on exist or no target file
    catdocArgs << targetFile;
    QProcess *process = new QProcess(this);
    qDebug()<<Q_FUNC_INFO<<" ( "<<__LINE__<<" )"<<outputFname;
    process->start("catdoc", catdocArgs);
    process->waitForFinished();
    QByteArray arr = process->readAllStandardOutput();
    qDebug()<<Q_FUNC_INFO<<" (((( "<<__LINE__<<" )))) "<<QString::fromUtf8(arr);
    //Saving data to file
    QFile outF(outputFname);
    if(!outF.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<Q_FUNC_INFO<<" (((( "<<__LINE__<<" )))) "<<" File creating error";
        return QVariant();
    }
    QTextStream outS(&outF);
    outS<<QString::fromUtf8(arr);

    qDebug()<<Q_FUNC_INFO<<" (((( "<<__LINE__<<" )))) "<<"Doc file successfully parsed";
    delete process;

    return QVariant();
}



// TODO implement it
//ParseXls
ParseXls::ParseXls(QObject *parent, const QString &/*filename*/):
     ParseDocXls(parent)
{

}

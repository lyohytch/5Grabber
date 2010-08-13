#include <QDebug>
#include <QProcess>
#include <QByteArray>
#include <QFile>

#include "parsedocxls.h"

ParseDocXls::ParseDocXls(QObject *parent) :
    QObject(parent)
{
}

QVariant ParseDocXls::convertingToTxt(const QString &cmd)
{
    // FIXME
    //Constructing output filename
    QString outputFname = targetFile;
    outputFname.chop(4);
    outputFname += ".txt";
    //TODO rework it when I have more time
    //Converting doc to txt
    //Check on exist or no target file
    docxlsArgs << targetFile;
    QProcess *process = new QProcess(this);
    qDebug()<<Q_FUNC_INFO<<" ( "<<__LINE__<<" )"<<outputFname;
    process->start(cmd, docxlsArgs);
    process->waitForFinished();
    QByteArray arr = process->readAllStandardOutput();
    //Saving data to file
    QFile outF(outputFname);
    if(!outF.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<Q_FUNC_INFO<<" (((( "<<__LINE__<<" )))) "<<" File creating error";
        return QVariant();
    }
    QTextStream outS(&outF);
    outS<<QString::fromUtf8(arr);

    qDebug()<<Q_FUNC_INFO<<" (((( "<<__LINE__<<" )))) "<<"file successfully converted";
    delete process;

    return QVariant();
}

void ParseDocXls::run()
{
    QVariant docxlsParseOut = ParseFile();
    emit docxlsParseFinished(docxlsParseOut.toString());
}


//**************************************ParseDoc
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
    convertingToTxt("catdoc");
    return QVariant("Parsing DOC file finished");
}




// TODO implement it
//**************************************************ParseXls
ParseXls::ParseXls(QObject *parent, const QString &filename):
     ParseDocXls(parent)
{
    qDebug()<<Q_FUNC_INFO<<" ( "<<__LINE__<<" ) : "<<filename;
    targetFile = filename;
    readConfig(".configMS");
}
void ParseXls::readConfig(const QString &filename)
{
    qDebug()<<Q_FUNC_INFO<<" ( "<<__LINE__<<" )";
    //TODO need to reading setting from file
}
QVariant ParseXls::ParseFile()
{
    qDebug()<<Q_FUNC_INFO<<" ( "<<__LINE__<<" )";
    convertingToTxt("xls2csv");
    return QVariant("Parsing XLS file finished");
}

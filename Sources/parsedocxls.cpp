#include <QDebug>
#include <QProcess>

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
    catdocArgs << filename;
    QProcess process;
    process.start("catdoc", catdocArgs);
    return QVariant();
}



// TODO implement it
//ParseXls
ParseXls::ParseXls(QObject *parent, const QString &filename):
     ParseDocXls(parent)
{

}

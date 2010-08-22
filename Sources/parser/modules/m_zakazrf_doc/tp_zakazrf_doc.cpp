#include <QDebug>
#include <QProcess>
#include <QByteArray>
#include <QFile>
#include <QFileInfo>

#include "tp_zakazrf_doc.h"

TP_zakazrf_doc::TP_zakazrf_doc()
{
    m_threadCounter = 0;
    m_signaller = new CParseSignaller();
}

TP_zakazrf_doc::~TP_zakazrf_doc()
{
    delete m_signaller;
}

bool TP_zakazrf_doc::init(int maxThreads, CDataStructure *data)
{
    m_maxThreads = maxThreads;
    m_data = data;
    return TRUE;
}

CParseSignaller* TP_zakazrf_doc::signaller()
{
    return m_signaller;
}

//QMap<QString,QVariant> TP_zakazrf::parse(CDataStructure* _data)
//{
//    QMap<QString,QVariant> result;
//    QString sdata(_data->read());
//    result = parse_html_auk(sdata);
//
//    for(int i = 0;  i < _data->childscCount(); i++)
//    {
//        CDataStructure* child= _data->childAt(i);
//        if(!child)
//        {
//            qCritical()<<"Fatal!!!! Childs is null";
//            continue;
//        }
//
//        switch(child->type())
//        {
//            case 0:
//            {
//                QString sdata(child->read());
//                result.unite(parse_html_lot(sdata));
//                break;
//            }
//            case 1:
//            {
//                break;
//            }
//            default:
//                break;
//        }
//    }
//
//    return result;
//}

bool TP_zakazrf_doc::run()
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<< "RUN PARSE TASK!!!";

    for (int i = 0; i < m_data->childscCount(); i++)
    {
        if (m_data->childAt(i)->type() == CDataStructure::eDataTypeDocument)
        {
            qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<< m_data->childAt(i)->url() << " type = " << m_data->childAt(i)->type();
            //docToTxt(m_data->childAt(i));
        }
    }

    m_signaller->onParseFinished();
    return TRUE;
}

void TP_zakazrf_doc::docToTxt(CDataStructure *p_data)
{
    QFile file(QString("%1.doc").arg(p_data->url().toString().section("=",1)));
    bool ok = file.open(QFile::WriteOnly);
    if (ok)
    {
        file.write(p_data->read());
        file.close();
        QFileInfo fileInfo(file);
        QStringList args;
        args.append("-t");
        args.append("-d utf-8");
        args.append(fileInfo.filePath());
        QProcess runner;
        runner.setReadChannel(QProcess::StandardOutput);
        runner.start("catdoc", args);
        runner.waitForFinished(15000); //15 sec
        QByteArray output = runner.readAllStandardOutput();
        qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<< "Output " << output.left(20);
    }
    file.remove();
}

//QMap<QString,QVariant> TP_zakazrf_doc::parse(CDataStructure* _data)
//{
//    QMap<QString,QVariant> result;
//    QString sdata(_data->read());
////    result = parse_html_auk(sdata);
//
//    for(int i = 0;  i < _data->childscCount(); i++)
//    {
//        CDataStructure* child= _data->childAt(i);
//        if(!child)
//        {
//            qCritical()<<"Fatal!!!! Childs is null";
//            continue;
//        }
//
//        switch(child->type())
//        {
//            case 0:
//            {
//                QString sdata(child->read());
////                result.unite(parse_html_lot(sdata));
//                break;
//            }
//            case 1:
//            {
//                break;
//            }
//            default:
//                break;
//        }
//    }
//
//    return result;
//}

//ParseDocXls::ParseDocXls(QObject *parent) :
//    QObject(parent)
//{
//    readConfig(".configMS");
//}
//
////ParseDoc
//ParseDoc::ParseDoc(QObject *parent, const QString &filename):
//     ParseDocXls(parent)
//{
//    qDebug()<<Q_FUNC_INFO<<" ( "<<__LINE__<<" ) : "<<filename;
//
//
//}
//
//void ParseDoc::readConfig(const QString &filename)
//{
//    qDebug()<<Q_FUNC_INFO<<" ( "<<__LINE__<<" )";
//}
//
//QVariant ParseDoc::ParseFile(const QString &filename)
//{
//    qDebug()<<Q_FUNC_INFO<<" ( "<<__LINE__<<" )";
//    // FIXME
//    QString fname = filename;
//    fname.chop(4);
//    fname += ".txt";
//    catdocArgs << filename;
//    QProcess *process = new QProcess(this);
//    qDebug()<<Q_FUNC_INFO<<" ( "<<__LINE__<<" )"<<fname;
//   // process->setStandardOutputFile(fname,QIODevice::ReadWrite);
//    process->execute("/usr/local/bin/catdoc", catdocArgs);
//    //process->waitForFinished();
//    QByteArray arr = process->readAllStandardOutput();
//    qDebug()<<Q_FUNC_INFO<<" (((( "<<__LINE__<<" )))) "<<arr;
//    delete process;
//
//    return QVariant();
//}
//
//
//
//// TODO implement it
////ParseXls
//ParseXls::ParseXls(QObject *parent, const QString &filename):
//     ParseDocXls(parent)
//{
//
//}

Q_EXPORT_PLUGIN2(TP_zakazrf_doc, TP_zakazrf_doc);

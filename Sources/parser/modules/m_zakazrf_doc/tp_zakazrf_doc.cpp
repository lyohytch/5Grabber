#include <QDebug>
#include <QProcess>
#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QRegExp>
#include <QMutex>

#include "tp_zakazrf_doc.h"
#include "constants.h"

TP_zakazrf_doc::TP_zakazrf_doc()
{
    m_threadCounter = 0;
    m_signaller = new CParseSignaller();
//    m_db = new DBmanager();
}

TP_zakazrf_doc::~TP_zakazrf_doc()
{
    delete m_signaller;
//    delete m_db;
}

bool TP_zakazrf_doc::init(int maxThreads, CDataStructure *data, DBmanager *db)
{
    m_maxThreads = maxThreads;
    m_data = data;
//    m_db->init();
    m_db = db;
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
    qDebug() << "RUN PARSE DOC TASK!!!" << m_data->root()->url().toString();

    for (int i = 0; i < m_data->childscCount(); i++)
    {
        CDataStructure* lot = m_data->childAt(i);
        //qDebug() << "CHILDS COUNT " << m_data->childscCount() << m_data->childAt(i)->url().toString();
        // Looking througth the all Lots
        for (int j = 0; j < lot->childscCount(); j++)
        {
            // Looking for the DOCs in Lots
            if (lot->childAt(j)->type() == CDataStructure::eDataTypeDocument)
            {
                docToXml(lot->childAt(j));
            }
        }
    }

    m_signaller->onParseFinished();
    return TRUE;
}

void TP_zakazrf_doc::docToXml(CDataStructure *p_data)
{
    QMutex mutex;
    bool valid = true;
    QFile file(QString("/tmp/%1.doc").arg(p_data->url().toString().section("=",1)));
    bool ok = file.open(QFile::WriteOnly);
    qDebug() << "OPEN FILE FOR WRITE is " << ok;
    if (ok)
    {
        file.write(p_data->read());
        file.close();
        QFileInfo fileInfo(file);
        qDebug() << "DOC Saved in " << file.fileName();

        QStringList args;
        args << "-x" << "db";
//        args.append("-t");
//        args.append("-d utf-8");
        args.append(fileInfo.filePath());
        //args << ">";
        //args.append(QString("%1.txt").arg(p_data->url().toString().section("=",1)));
        QProcess runner;
        runner.setReadChannel(QProcess::StandardOutput);
//        runner.start("catdoc", args);
        runner.start("antiword", args);
        runner.waitForFinished(15000); //15 sec
        QByteArray output = runner.readAllStandardOutput();
        QByteArray err = runner.readAllStandardError();
        if (err.isEmpty())
        {
            QString info = findProviding(output);
            qDebug() << "FINDINGS\n" <<info;
            QVariantMap db_data;
            db_data.insert("info", info);
            db_data.insert("url", p_data->url().toString());
            db_data.insert("id_reduction", p_data->root()->url().toString().section("=", 1));
            mutex.lock();
                m_db->writeDoc(db_data);
            mutex.unlock();
            qDebug() << "Output " << output.left(50);
        }
        else
        {
            valid = FALSE;
            qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<< "Error" << err;
        }
    }
    file.remove();
}

QString TP_zakazrf_doc::findProviding(const QByteArray &source)
{
   QTextStream stream(source);
   QString sourceStr(stream.readAll());
   sourceStr = sourceStr.remove(QRegExp("\n|\t|\r|\a"));
   QString found;
   QRegExp regexp(QString("<[^<]*>[^<]*<[^<]*>"), Qt::CaseInsensitive);
   for (int pos = regexp.indexIn(sourceStr); pos >= 0; pos = regexp.indexIn(sourceStr,pos + 1))
   {
       QString tmp = regexp.capturedTexts().at(0);
       if (tmp.contains(QTextStream("обеспечен").readAll(),Qt::CaseInsensitive))
       {
           found.append(tmp + "\n");
       }
   }

   return found;
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

#include "tparsethread.h"

#include <QDebug>

TParseThread::TParseThread()
{
//    m_id = _id;
//    m_url = _url;
//    m_data = _data;

    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO;

    if (m_data->hasChilds())
    {
        for (int i = 0; i < m_data->childscCount(); i++)
        {
            if (m_data->childAt(i)->type() == CDataStructure::eDataTypePage)
            {
//                QFile file(QString("%1.txt").arg(m_data->childAt(i)->url().toString().section("=",1)));
//                QFileInfo fileInfo(file);
//                bool ok = file.open(QIODevice::WriteOnly);
//                qDebug() << __FUNCTION__ << "OPEN file " << file.fileName() << ok;
//                qDebug() << __FUNCTION__ << "BYTES read " << file.write(m_data->childAt(i)->read());
//                file.close();
                continue;
            }
            else if (m_data->childAt(i)->type() == CDataStructure::eDataTypeDocument)
            {
//                QFile file(QString("%1.doc").arg(m_data->childAt(i)->url().toString().section("=",1)));
//                QFileInfo fileInfo(file);
//                bool ok = file.open(QIODevice::WriteOnly);
//                qDebug() << __FUNCTION__ << "OPEN file " << file.fileName() << ok;
//                qDebug() << __FUNCTION__ << "BYTES read " << file.write(m_data->childAt(i)->read());
//                file.close();
//                QProcess runner;
//                QStringList args;
//                args.append("-d utf-8");
//                args.append("-t");
//                args.append(fileInfo.absoluteFilePath());
//                runner.setReadChannel(QProcess::StandardOutput);
//                runner.start("catdoc", args);
//                runner.waitForFinished(15000); // wait for catdoc no more than 15 sec
//                QByteArray output = runner.readAllStandardOutput();
////                qDebug() << __FUNCTION__ << "Catdoc OUTPUT" << output;
//
//                ok = file.remove();
//                qDebug() << __FUNCTION__ << "REMOVE file " << ok;
//                break;
            }
        }
    }

//    if(m_url == URL1)
//    {
//        m_path = PATH_MODULES + SO1;
//    }
//    else
//    {
//        m_path = "";
//    }
}

void TParseThread::run()
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<":"<< "RUN";
//    QPluginLoader task(m_path);
//    if( FALSE == task.load() )
//    {
//        qDebug() << "run: load error"<<task.errorString();
//        return;
//    }
//
//    TP_Task* plugin=qobject_cast<TP_Task *>(task.instance());
//    m_result = plugin->parse(m_data);
//    m_error=NO_ERROR;

}

#include "cparsethread.h"

#include <QDebug>
#include <QPluginLoader>

CParseThread::CParseThread()
{
    // nothing
}

void CParseThread::run()
{
    qDebug()<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO << "============ HELLO from thread "<<this->currentThreadId();
}

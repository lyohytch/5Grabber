#include "crecievetask_test.h"
#include <constants.h>

CRecieveTask_test::CRecieveTask_test()
{

}

QString CRecieveTask_test::taskHost() const
{
    return QString("testhost");
}

bool CRecieveTask_test::init(int maxThreads, const siterules_ti &rule)
{
    Q_UNUSED(maxThreads)
    Q_UNUSED(rule)
    m_signaller = new CRecieveTaskSignaller;
    return true;
}

bool CRecieveTask_test::run()
{
    qDebug()<<"RUNNED!!!!!!!!!!!!!!!!!!!";
    return true;
}

CRecieveTaskSignaller* CRecieveTask_test::signaller()
{
    return m_signaller;
}

Q_EXPORT_PLUGIN2(CRecieveTask_test, CRecieveTask_test);

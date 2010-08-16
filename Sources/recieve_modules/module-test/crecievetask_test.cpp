#include "crecievetask_test.h"

CRecieveTask_test::CRecieveTask_test()
{

}

bool CRecieveTask_test::init(int maxThreads, CDataBaseHandler *, const siterules_ti &rule)
{
    return true;
}

bool CRecieveTask_test::run()
{
    qDebug("RUNNED!!!!!!!!!!!!!!!!!!!");
    return true;
}
CRecieveTaskSignaller* CRecieveTask_test::signaller()
{
    m_signaller = new CRecieveTaskSignaller;
    return m_signaller;
}

CDataBaseHandler* CRecieveTask_test::database()
{
    return NULL;
}

Q_EXPORT_PLUGIN2(CRecieveTask_test, CRecieveTask_test);

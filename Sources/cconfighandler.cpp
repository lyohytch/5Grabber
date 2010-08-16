#include "cconfighandler.h"

CConfigHandler::CConfigHandler(QObject *parent) :
    QObject(parent)
{
}

bool CConfigHandler::load(const QUrl&)
{
    loadDefaults();
    return true;
}

bool CConfigHandler::loadDefaults()
{
    m_configValues.insert(QString("core_max_threads"), QVariant(2));
}



QVariant CConfigHandler::valueOf(const QString& key)
{
    return m_configValues.value(key);
}


CConfigHandler g_config;

#include "cconfighandler.h"

#include <QFile>
#include <constants.h>

QSettings* CConfigHandler::m_settings=NULL;

bool CConfigHandler::load(const QString &path)
{
    if(!m_settings)
    {
        delete m_settings;
    }
    m_settings = new QSettings(path, QSettings::IniFormat);
    return (m_settings->status()==QSettings::NoError);
}

QSettings* CConfigHandler::instance(bool &isOk)
{
    if(!m_settings)
    {
        qWarning()<<"m_settings is NULL creating new instance";
        m_settings = new QSettings(cConfigUrl, QSettings::IniFormat);
    }
    isOk=(m_settings->status()==QSettings::NoError);
    return m_settings;
}

QSettings* CConfigHandler::instance()
{
    if(!m_settings)
    {
        qWarning()<<"m_settings is NULL creating new instance";
        m_settings = new QSettings(cConfigUrl, QSettings::IniFormat);
    }
    return m_settings;
}


bool CConfigHandler::loadSites(QMap<QUrl, QVariantList>& resultTable)
{
    if(!m_settings)
    {
        qWarning()<<"m_settings is NULL";
        return false;
    }

    QVariantList options; //thats empty for now by default
    QString sitesConfigPath=(m_settings->value("main/sites_config","./config/sites.lst")).toString();
    QFile sitesConfig(sitesConfigPath);
    sitesConfig.open(QFile::ReadOnly);

    for(QByteArray data=sitesConfig.readLine();!data.isEmpty();data=sitesConfig.readLine())
    {
        QString str(data);
        QUrl url(str.remove('\n'));
        if(!url.isValid() || str.at(0)=='#')
        {
            continue;
        }
        resultTable.insert(url,options);
    }

    return true;
}

void CConfigHandler :: flush()
{
    if(m_settings)
    {
        delete m_settings;
        m_settings=NULL;
    }
}

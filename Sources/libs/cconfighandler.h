#ifndef CCONFIGHANDLER_H
#define CCONFIGHANDLER_H

#include <QObject>
#include <QVariant>
#include <QString>
#include <QVariantMap>
#include <QVariantList>

#include "constants.h"
class CConfigHandler : public QObject
{
Q_OBJECT
public:
    CConfigHandler(QObject *parent = 0);
    bool load(const QUrl&);
    void flush(){};
    QVariant valueOf(const QString& key);
    siterules_t loadSites()
    {
        siterules_t rules;
        QUrl tmpSite("http://zakazrf.ru/");
        QVariantList tmpRules;
        tmpRules.append(QVariant("/LotList.aspx?Subject="));
        rules.insert(tmpSite,tmpRules);
        return rules;
    }
signals:

public slots:

protected:
    bool loadDefaults();
    bool loadConfig(const QUrl&){return true;};
    bool writeLine(const QString& line){return true;};
    bool validateValue(const QString& key, const QVariant& value){return true;};

    QVariantMap m_configValues;
};

extern CConfigHandler g_config;

#endif // CCONFIGHANDLER_H
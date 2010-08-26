#ifndef CCONFIGHANDLER_H
#define CCONFIGHANDLER_H

#include <QObject>
#include <QSettings>

#include "constants.h"
class CConfigHandler
{
public:
    static bool load(const QString &path);
    static QSettings* instance();
    static QSettings* instance(bool &isOk);
    static bool loadSites(QMap<QUrl, QVariantList>& resultTable);
    static void flush();
private:
    static QSettings *m_settings;
};

#endif // CCONFIGHANDLER_H

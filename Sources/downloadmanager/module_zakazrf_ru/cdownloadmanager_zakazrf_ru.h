#ifndef CDOWNLOADMANAGER_ZAKAZRF_RU_H
#define CDOWNLOADMANAGER_ZAKAZRF_RU_H

#include <QObject>
#include <constants.h>

class CDownloadManager_zakazrf_ru : public QObject
{
    Q_OBJECT
public:
    CDownloadManager_zakazrf_ru();
    virtual ~CDownloadManager_zakazrf_ru();
    bool init();
    QUrlList getUrls();
};

#endif // CDOWNLOADMANAGER_ZAKAZRF_RU_H

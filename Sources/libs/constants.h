#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QString>
#include <QUrl>
#include <QMap>
#include <QVariantList>
#include <QDebug>

#ifdef TIME_STAMPS
#include <QDateTime>
#endif

static const QString cConfigUrl("./config/main.ini");
typedef QMap<QUrl, QVariantList> siterules_t;
typedef QMapIterator<QUrl, QVariantList> siterules_ti;
typedef QList<QUrl> QUrlList;
#define qDebug() qDebug()<<"DEBUG -"<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<": "
#define qCritical() qCritical()<<"CRITICAL -"<<__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<": "
#define qWarning() qWarning()<<"WARNING -"__FILE__<<"("<<__LINE__<<") "<<Q_FUNC_INFO<<": "

#define getConfigurationValue(X, Y) (CConfigHandler::instance()->value(X, Y))


#endif // CONSTANTS_H

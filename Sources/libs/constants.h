#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QString>
#include <QUrl>
#include <QMap>
#include <QVariantList>

static const QUrl cConfigUrl("./config");
typedef QMap<QUrl, QVariantList> siterules_t;
typedef QMapIterator<QUrl, QVariantList> siterules_ti;

#endif // CONSTANTS_H

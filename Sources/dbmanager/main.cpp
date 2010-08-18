#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "dbmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DBmanager dbman;
    dbman.init();
    QVariantMap map;
    qDebug() << "######## insert query" << dbman.write(map);

    return a.exec();
}

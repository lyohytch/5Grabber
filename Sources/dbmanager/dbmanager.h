#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QMap>
#include <QVariant>

class DBmanager
{
public:
    DBmanager();
    // DBmanager(const QString& db_name = "zakazrf");
    ~DBmanager();
    bool init();
    bool is_open();
    bool write(QVariantMap &data);
    bool read();

private:
    QString m_dbName;
    QString m_dbHost;
    const QString m_dbUser;
    const QString m_dbPass;
    QString m_dbPort;
    QSqlDatabase m_db;
    bool m_status;
};

#endif // DBMANAGER_H

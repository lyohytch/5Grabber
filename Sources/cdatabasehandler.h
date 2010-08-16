#ifndef CDATABASEHANDLER_H
#define CDATABASEHANDLER_H

#include <QObject>
#include <QUrl>
#include <QByteArray>
#include <QFile>

class CDataBaseHandler : public QObject
{
Q_OBJECT
public:
    CDataBaseHandler(QObject *parent = 0);
    bool open(const QUrl&);
    void close();
    int type();
    qint64 write(const QByteArray& data);
    QByteArray read();

    bool isBusy();
signals:

public slots:
private:
    int m_type;
    bool m_busy;
    QFile m_file;
};

#endif // CDATABASEHANDLER_H

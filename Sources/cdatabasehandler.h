#ifndef CDATABASEHANDLER_H
#define CDATABASEHANDLER_H

#include <QObject>
#include <QUrl>
#include <QByteArray>

class CDataBaseHandler : public QObject
{
Q_OBJECT
public:
    CDataBaseHandler(QObject *parent = 0);
    bool open(const QUrl&) {return true;};
    void close();
    int type();
    int write(const QByteArray& data);
    int read(QByteArray& data) const;

    bool isBusy();
signals:

public slots:
private:
    int m_type;
    bool m_busy;
};

#endif // CDATABASEHANDLER_H

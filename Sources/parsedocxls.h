#ifndef PARSEDOCXLS_H
#define PARSEDOCXLS_H

#include <QObject>
#include <QStringList>
#include <QVariant>

class ParseDocXls : public QObject
{
Q_OBJECT
public:
    explicit ParseDocXls(QObject *parent = 0);
    virtual void readConfig(const QString &/*cfile*/) {}
    virtual QVariant ParseFile() { return QVariant();}
    QStringList catdocArgs;
    QStringList configParams;
signals:

public slots:
protected:
    QString targetFile;

};

class ParseDoc : public ParseDocXls
{
Q_OBJECT
public:
    ParseDoc(QObject *parent = 0, const QString &filename = 0);
    virtual void readConfig(const QString &filename);
    virtual QVariant ParseFile();
signals:

public slots:

};

class ParseXls : public ParseDocXls
{
Q_OBJECT
public:
    ParseXls(QObject *parent = 0, const QString &filename = 0);
signals:

public slots:

};
#endif // PARSEDOCXLS_H

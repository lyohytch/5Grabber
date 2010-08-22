#ifndef PARSEDOCXLS_H
#define PARSEDOCXLS_H

#include <QObject>
#include <QStringList>
#include <QVariant>
#include <QRunnable>

class ParseDocXls : public QObject, public QRunnable
{
Q_OBJECT
public:
    explicit ParseDocXls(QObject *parent = 0);
    virtual void readConfig(const QString &/*cfile*/) {}
    virtual QVariant ParseFile() { return QVariant();}
    virtual void run();
    QVariant convertingToTxt(const QString &cmd);
    QStringList docxlsArgs; //list of files to convert
    QStringList configParams;

signals:
    void docxlsParseFinished(QString);
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
    QVariant parseProcess(const QVariant & outArr);
signals:

public slots:

};

class ParseXls : public ParseDocXls
{
Q_OBJECT
public:
    ParseXls(QObject *parent = 0, const QString &filename = 0);
    virtual void readConfig(const QString &filename);
    virtual QVariant ParseFile();
    QVariant parseProcess(const QVariant & outArr);
signals:

public slots:

};
#endif // PARSEDOCXLS_H

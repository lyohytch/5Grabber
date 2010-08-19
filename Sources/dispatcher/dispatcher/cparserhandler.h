#ifndef CPARSERHANDLER_H
#define CPARSERHANDLER_H

#include <QObject>

#include <cdatastructure.h>
#include <tparsereceiver.h>

class CParserHandler : public QObject
{
Q_OBJECT
public:
    explicit CParserHandler(QObject *parent = 0);
    ~CParserHandler();
    bool init();
signals:

public slots:
    void startParsing(CDataStructure* data, const QUrl &url);
    void onParceFinished(int _error,const QByteArray& _url);
private:
    TParseReceiver *m_parcer;
};

#endif // CPARSERHANDLER_H

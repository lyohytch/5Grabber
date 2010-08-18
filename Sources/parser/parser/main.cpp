#include <QApplication>
#include "tparsereceiver.h"
#include "cdatastructure.h"
#include <QMap>
#include <QVariant>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QUrl>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TParseReceiver parser;

    QUrl b("http://zakazrf.ru");
    CDataStructure _data(b);

    QByteArray n("http://zakazrf.ru");
    parser.parse(&_data,n);


    return a.exec();
}

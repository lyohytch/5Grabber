#ifndef CDATASTRUCTURE_H
#define CDATASTRUCTURE_H

#include <QObject>
#include <QList>
#include <QUrl>
#include <QRegExp>
#include <QStringList>

class CDataStructure : public QObject
{
Q_OBJECT
public:
    enum DataTypes
    {
        eDataTypePage=0,
        eDataTypeDocument=1
    };

    CDataStructure(QUrl url, QObject *parent = 0);
    ~CDataStructure();

    bool hasChilds() {return (!m_childs.isEmpty());}
    int childscCount() {return m_childs.count();}
    CDataStructure* childAt(int i);
    void appendChild(CDataStructure* child);
    void removeChild(CDataStructure*);

    void setType(int type){m_type=(DataTypes)type;}
    int type() const {return (int)m_type;}

    void write(QByteArray& data);
    QByteArray read() const {return m_data;}

    void flush();

    QUrl url() const {return m_url;}
    QStringList findLinks(QList<QRegExp>& regexps);
    bool isFinished();
    bool isDone() {return m_isDone;}
    void done() {m_isDone=true;}

    bool isRoot() {return m_isRoot;}
    void setRoot() {m_isRoot=true;}
private:
    DataTypes m_type;
    QList<CDataStructure* > m_childs;
    QByteArray m_data;
    QUrl m_url;
    bool m_isDone;
    bool m_isRoot;
};

#endif // CDATASTRUCTURE_H

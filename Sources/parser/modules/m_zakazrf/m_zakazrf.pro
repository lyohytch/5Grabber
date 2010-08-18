TARGET = m_zakazrf
TEMPLATE = lib
QT += webkit
DESTDIR += ../../bin
INCLUDEPATH += ../
HEADERS += tp_zakazrf.h \
           ../tp_task.h \
           ../cdatastructure.h
SOURCES += tp_zakazrf.cpp \
           ../cdatastructure.cpp

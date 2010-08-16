TARGET = module-zakazrf_ru
TEMPLATE = lib
DESTDIR += ../../bin/recieve-modules
QT += network
INCLUDEPATH += ../ \
    ../../ \
    ../crecievethread/
HEADERS += crecievetask-zakazrf_ru.h \
    ../crecievetask.h \
    ../crecievetasksignaller.h \
    ../crecievethread/crecivethread.h \
    ../../datastructure/cdatastructure.h
SOURCES += crecievetask-zakazrf_ru.cpp \
    ../crecievethread/crecivethread.cpp \
    ../../datastructure/cdatastructure.cpp

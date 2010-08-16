TARGET = module-test
TEMPLATE = lib
DESTDIR += ../../bin/recieve-modules
INCLUDEPATH += ../ \
    ../../
HEADERS += crecievetask_test.h \
    ../crecievetask.h \
    ../crecievetasksignaller.h
SOURCES += crecievetask_test.cpp

QT += network
TARGET = grabber
TEMPLATE = app
DESTDIR = ./bin
INCLUDEPATH += ./recieve_modules
HEADERS += cmaindispatcher.h \
    cconfighandler.h \
    cdatabasehandler.h \
    loadbalancer.h \
    constants.h \
    cparserhandler.h \
    parsedocxls.h \
    recieve_modules/crecievetask.h \
    recieve_modules/crecievetasksignaller.h \
    datastructure/cdatastructure.h
SOURCES += cmaindispatcher.cpp \
    cconfighandler.cpp \
    cdatabasehandler.cpp \
    loadbalancer.cpp \
    cparserhandler.cpp \
    main.cpp \
    parsedocxls.cpp \
    datastructure/cdatastructure.cpp

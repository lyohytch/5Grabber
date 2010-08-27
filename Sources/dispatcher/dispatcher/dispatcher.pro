TEMPLATE = app
TARGET = grabber

QT += core
QT += gui
QT += network
QT += phonon
QT += xml
QT += xmlpatterns
QT += webkit
QT += dbus
QT += sql


CONFIG += console
CONFIG -= app_bundle

GBR_SOURCE_ROOT=../..
include($$GBR_SOURCE_ROOT/paths.pri)

DEFINES += TIME_STAMPS

DESTDIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BIN_PATH

OBJECTS_DIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BUILD_PATH/$$TARGET/obj
MOC_DIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BUILD_PATH/$$TARGET/moc

INCLUDEPATH += $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH \
               $$GBR_SOURCE_ROOT/$$GRB_DSP_MODULES_PATH \
               $$GBR_SOURCE_ROOT/$$GRB_PARSER_PATH \
               $$GBR_SOURCE_ROOT/$$GRB_PARSER_MODULE_PATH \
               $$GBR_SOURCE_ROOT/$$GRB_DBMANAGER_PATH


LIBS +=-L$$PWD/$$GBR_SOURCE_ROOT/$$GRB_LIBS_BIN_PATH -lparser -ldbmanager

HEADERS += cmaindispatcher.h \
    loadbalancer.h \
    cparserhandler.h \
    $$GBR_SOURCE_ROOT/$$GRB_DSP_MODULES_PATH/crecievetask.h \
    $$GBR_SOURCE_ROOT/$$GRB_DSP_MODULES_PATH/crecievetasksignaller.h \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH/constants.h \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH/cconfighandler.h \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH/cdatastructure.h \
    $$GBR_SOURCE_ROOT/$$GRB_PARSER_PATH/tparsereceiver.h \
    $$GBR_SOURCE_ROOT/$$GRB_PARSER_PATH/cparsedispatcher.h \
    $$GBR_SOURCE_ROOT/$$GRB_PARSER_PATH/cparsethread.h \
    $$GBR_SOURCE_ROOT/$$GRB_PARSER_MODULE_PATH/tp_task.h \
    $$GBR_SOURCE_ROOT/$$GRB_PARSER_MODULE_PATH/cparsesignaller.h \
    $$GBR_SOURCE_ROOT/$$GRB_DBMANAGER_PATH/dbmanager.h

SOURCES += cmaindispatcher.cpp \
    loadbalancer.cpp \
    cparserhandler.cpp \
    main.cpp \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_SRC_PATH/cconfighandler.cpp \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_SRC_PATH/cdatastructure.cpp \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_SRC_PATH/constants.cpp

DEFINES += RUN_TIME_PATH="\\\"$$PWD/$$GBR_SOURCE_ROOT/$$GRB_BIN_PATH\\\""

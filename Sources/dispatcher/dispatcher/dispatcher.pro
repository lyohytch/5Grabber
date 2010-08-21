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


CONFIG += console
CONFIG -= app_bundle

GBR_SOURCE_ROOT=../..
include($$GBR_SOURCE_ROOT/paths.pri)

DESTDIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BIN_PATH

OBJECTS_DIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BUILD_PATH/$$TARGET/obj
MOC_DIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BUILD_PATH/$$TARGET/moc

INCLUDEPATH += $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH \
               $$GBR_SOURCE_ROOT/$$GRB_DSP_MODULES_PATH \
               $$GBR_SOURCE_ROOT/$$GRB_PARSER_PATH

LIBS +=-L$$PWD/$$GBR_SOURCE_ROOT/$$GRB_LIBS_BIN_PATH -lparser

HEADERS += cmaindispatcher.h \
    loadbalancer.h \
    cparserhandler.h \
    $$GBR_SOURCE_ROOT/$$GRB_DSP_MODULES_PATH/crecievetask.h \
    $$GBR_SOURCE_ROOT/$$GRB_DSP_MODULES_PATH/crecievetasksignaller.h \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH/constants.h \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH/cconfighandler.h \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH/cdatastructure.h \
    $$GBR_SOURCE_ROOT/$$GRB_PARSER_PATH/tparsereceiver.h

SOURCES += cmaindispatcher.cpp \
    loadbalancer.cpp \
    cparserhandler.cpp \
    main.cpp \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_SRC_PATH/cconfighandler.cpp \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_SRC_PATH/cdatastructure.cpp

DEFINES += RUN_TIME_PATH="\\\"$$PWD/$$GBR_SOURCE_ROOT/$$GRB_BIN_PATH\\\""
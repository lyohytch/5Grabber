TARGET = dbmanager
TEMPLATE = lib

GBR_SOURCE_ROOT=..
include($$GBR_SOURCE_ROOT/paths.pri)

QT += sql
#QT -= gui

#CONFIG += console
#CONFIG -= app_bundle
#TEMPLATE = app
#SOURCES += main.cpp \
#    dbmanager.cpp \
#HEADERS += dbmanager.h


DEFINES += fPIC
SOURCES += dbmanager.cpp
HEADERS += dbmanager.h



CONFIG += console
CONFIG -= app_bundle

DESTDIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_LIBS_BIN_PATH
OBJECTS_DIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BUILD_PATH/$$TARGET/obj
MOC_DIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BUILD_PATH/$$TARGET/moc
INCLUDEPATH += $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH \
    $$GBR_SOURCE_ROOT/$$GRB_DSP_MODULES_PATH
#INCLUDEPATH += ../modules/
#HEADERS += tparsereceiver.h \
#    ../modules/tp_task.h \
#    ../modules/cparsesignaller.h \
#    $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH/cdatastructure.h \
#    $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH/tparsethread.h
#SOURCES += tparsereceiver.cpp \
#    $$GBR_SOURCE_ROOT/$$GRB_LIBS_SRC_PATH/cdatastructure.cpp \
#    $$GBR_SOURCE_ROOT/$$GRB_LIBS_SRC_PATH/tparsethread.cpp

# -------------------------------------------------
# Project created by QtCreator 2010-08-16T19:42:34
# -------------------------------------------------
QT += core
QT += gui
QT += network
QT += phonon
QT += xml
QT += xmlpatterns
QT += webkit
QT += dbus
TARGET = parser
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = lib

GBR_SOURCE_ROOT = ../..
include($$GBR_SOURCE_ROOT/paths.pri)

DESTDIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_LIBS_BIN_PATH
OBJECTS_DIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BUILD_PATH/$$TARGET/obj
MOC_DIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BUILD_PATH/$$TARGET/moc
INCLUDEPATH += $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH \
    $$GBR_SOURCE_ROOT/$$GRB_DSP_MODULES_PATH
INCLUDEPATH += ../modules/
HEADERS += tparsereceiver.h \
    cparsedispatcher.h \
    cparsethread.h \
    ../modules/tp_task.h \
    ../modules/cparsesignaller.h \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH/cdatastructure.h \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH/tparsethread.h
SOURCES += tparsereceiver.cpp \
    cparsedispatcher.cpp \
    cparsethread.cpp \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_SRC_PATH/cdatastructure.cpp \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_SRC_PATH/tparsethread.cpp

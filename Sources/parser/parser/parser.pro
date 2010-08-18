# -------------------------------------------------
# Project created by QtCreator 2010-08-16T19:42:34
# -------------------------------------------------
QT += core
QT += webkit
QT += gui

TARGET = parser
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = lib

GBR_SOURCE_ROOT=../..
include($$GBR_SOURCE_ROOT/paths.pri)

DESTDIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_LIBS_BIN_PATH

OBJECTS_DIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BUILD_PATH/$$TARGET/obj
MOC_DIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BUILD_PATH/$$TARGET/moc

INCLUDEPATH += $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH \
    $$GBR_SOURCE_ROOT/$$GRB_DSP_MODULES_PATH

INCLUDEPATH += ../modules/

HEADERS += tparsereceiver.h \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH/cdatastructure.h

SOURCES += tparsereceiver.cpp \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_SRC_PATH/cdatastructure.cpp


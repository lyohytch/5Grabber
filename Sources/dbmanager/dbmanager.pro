TARGET = dbmanager
TEMPLATE = lib
GBR_SOURCE_ROOT = ..
include($$GBR_SOURCE_ROOT/paths.pri)

QT += sql
# QT -= gui

# TEMPLATE = app
# SOURCES += main.cpp \
#    dbmanager.cpp \
# HEADERS += dbmanager.h
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

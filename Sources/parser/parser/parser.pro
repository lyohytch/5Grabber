#-------------------------------------------------
#
# Project created by QtCreator 2010-08-16T19:42:34
#
#-------------------------------------------------

QT       += core
QT       += webkit
QT       += gui

TARGET = parser
CONFIG   += console
CONFIG   -= app_bundle
INCLUDEPATH += ../modules/

TEMPLATE = lib


SOURCES += main.cpp \
    tparsereceiver.cpp \
    cdatastructure.cpp

HEADERS += \
    tparsereceiver.h \
    cdatastructure.h

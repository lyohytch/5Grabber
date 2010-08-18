QT += sql
QT -= gui
TARGET = DBmanager
#CONFIG += console
#CONFIG -= app_bundle
#TEMPLATE = app
#SOURCES += main.cpp \
#    dbmanager.cpp \
#HEADERS += dbmanager.h

TEMPLATE = lib
DEFINES += fPIC
SOURCES += dbmanager.cpp
HEADERS += dbmanager.h

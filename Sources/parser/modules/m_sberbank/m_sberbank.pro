TARGET = m_sberbank
TEMPLATE = lib

QT += sql

GBR_SOURCE_ROOT = ../../..
include($$GBR_SOURCE_ROOT/paths.pri)

DESTDIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_PARSE_MODULES_BIN_PATH

OBJECTS_DIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BUILD_PATH/$$TARGET/obj
MOC_DIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BUILD_PATH/$$TARGET/moc

INCLUDEPATH += $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH \
    $$GBR_SOURCE_ROOT/$$GRB_DSP_MODULES_PATH \
    $$GBR_SOURCE_ROOT/$$GRB_DBMANAGER_PATH
INCLUDEPATH += ../

HEADERS += tp_sberbank.h \
    ../tp_task.h \
    ../cparsesignaller.h \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH/cdatastructure.h \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH/tparsethread.h \
    $$GBR_SOURCE_ROOT/$$GRB_DBMANAGER_PATH/dbmanager.h

SOURCES += tp_sberbank.cpp \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_SRC_PATH/cdatastructure.cpp \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_SRC_PATH/tparsethread.cpp

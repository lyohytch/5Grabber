TARGET = module-sberbank-ast_ru
TEMPLATE = lib

QT += network

#DEFINES += RUN_ALL_TASKS

GBR_SOURCE_ROOT=../../..
include($$GBR_SOURCE_ROOT/paths.pri)

DESTDIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_RECIEVE_MODULES_BIN_PATH

OBJECTS_DIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BUILD_PATH/$$TARGET/obj
MOC_DIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BUILD_PATH/$$TARGET/moc

INCLUDEPATH += $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH \
    $$GBR_SOURCE_ROOT/$$GRB_DSP_MODULES_PATH

HEADERS += crecievetask-sberbank_ast_ru.h \
    ../crecievetask.h \
    ../crecievetasksignaller.h \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH/crecivethread.h \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH/cdatastructure.h
SOURCES += crecievetask-sberbank_ast_ru.cpp \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_SRC_PATH/crecivethread.cpp \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_SRC_PATH/cdatastructure.cpp
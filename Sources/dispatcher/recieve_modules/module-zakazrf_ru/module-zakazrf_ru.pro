TARGET = module-zakazrf_ru
TEMPLATE = lib

QT += network

#DEFINES += RUN_ALL_TASKS
DEFINES += TIME_STAMPS
GBR_SOURCE_ROOT=../../..
include($$GBR_SOURCE_ROOT/paths.pri)

DESTDIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_RECIEVE_MODULES_BIN_PATH

OBJECTS_DIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BUILD_PATH/$$TARGET/obj
MOC_DIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BUILD_PATH/$$TARGET/moc

INCLUDEPATH += $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH \
    $$GBR_SOURCE_ROOT/$$GRB_DSP_MODULES_PATH \
    $$GBR_SOURCE_ROOT/$$GRB_DWL_MODULES_PATH \
    $$GBR_SOURCE_ROOT/$$GRB_DWL_MODULES_PATH/module_zakazrf_ru

LIBS += -L$$GBR_SOURCE_ROOT/$$GRB_LIBS_BIN_PATH -ldownloadmanager-zakazrf_ru
HEADERS += crecievetask-zakazrf_ru.h \
    ../crecievetask.h \
    ../crecievetasksignaller.h \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH/crecivethread.h \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH/cdatastructure.h \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH/cconfighandler.h

SOURCES += crecievetask-zakazrf_ru.cpp \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_SRC_PATH/crecivethread.cpp \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_SRC_PATH/cdatastructure.cpp \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_SRC_PATH/cconfighandler.cpp


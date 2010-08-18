TARGET = module-test
TEMPLATE = lib

GBR_SOURCE_ROOT=../../..
include($$GBR_SOURCE_ROOT/paths.pri)

DESTDIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_RECIEVE_MODULES_BIN_PATH

OBJECTS_DIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BUILD_PATH/$$TARGET/obj
MOC_DIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BUILD_PATH/$$TARGET/moc

INCLUDEPATH += $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH \
    $$GBR_SOURCE_ROOT/$$GRB_DSP_MODULES_PATH

HEADERS += crecievetask_test.h \
    ../crecievetask.h \
    ../crecievetasksignaller.h
SOURCES += crecievetask_test.cpp

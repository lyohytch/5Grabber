TARGET = downloadmanager-zakazrf_ru
TEMPLATE = lib

GBR_SOURCE_ROOT=../..
include($$GBR_SOURCE_ROOT/paths.pri)

DESTDIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_LIBS_BIN_PATH

OBJECTS_DIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BUILD_PATH/$$TARGET/obj
MOC_DIR = $$PWD/$$GBR_SOURCE_ROOT/$$GRB_BUILD_PATH/$$TARGET/moc

INCLUDEPATH += .. \
    $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH \
    $$GBR_SOURCE_ROOT/$$GRB_DSP_MODULES_PATH \
    $$GBR_SOURCE_ROOT/$$GRB_DWL_MODULES_PATH

HEADERS += $$GBR_SOURCE_ROOT/$$GRB_LIBS_PATH/cconfighandler.h \
    cdownloadmanager_zakazrf_ru.h
#    $$GBR_SOURCE_ROOT/$$GRB_DWL_MODULES_PATH/cdownloadmanager.h
SOURCES += $$GBR_SOURCE_ROOT/$$GRB_LIBS_SRC_PATH/cconfighandler.cpp \
    cdownloadmanager_zakazrf_ru.cpp


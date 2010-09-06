GRB_BUILD_GLOBAL_PATH=build
GRB_BUILD_PATH=$$GRB_BUILD_GLOBAL_PATH/tmp

CONFIG(release, debug|release):CONFIG_PATH=release
CONFIG(debug, debug|release):CONFIG_PATH=debug

GRB_BIN_PATH=$$GRB_BUILD_GLOBAL_PATH/bin/$$CONFIG_PATH
GRB_MODULES_BIN_PATH=$$GRB_BIN_PATH/modules
GRB_RECIEVE_MODULES_BIN_PATH=$$GRB_MODULES_BIN_PATH/recieve
GRB_PARSE_MODULES_BIN_PATH=$$GRB_MODULES_BIN_PATH/parse
GRB_LIBS_BIN_PATH=$$GRB_BIN_PATH/libs

GRB_LIBS_PATH=libs
GRB_LIBS_SRC_PATH=$$GRB_LIBS_PATH/src

#paths only for dispatcher

GRB_DSP_MODULES_PATH=dispatcher/recieve_modules
GRB_DWL_MODULES_PATH=downloadmanager/downloadmanager_modules

#parser paths
GRB_PARSER_PATH=parser/parser
GRB_PARSER_MODULE_PATH=parser/modules

#parser paths
GRB_DBMANAGER_PATH=dbmanager

DEFINES += TIME_STAMPS



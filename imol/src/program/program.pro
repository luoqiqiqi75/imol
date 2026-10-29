# ------------------------------------------------------------
#
# IMOL (Infinite Modules On Line)
#
# @brief: Main entrance program of imol modules,
# @author: LuoQi
# @created at: 2019-06-14T11:49:29
#
# ------------------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = imolProgram
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

# user defines
CONFIG(debug, debug|release) {
#DEFINES += LOG_ENABLE_DETAILS
} else {
DEFINES += LOG_MIN_Q_LEVEL=IMOL_LOG_LEVEL_INFO
DEFINES += LOG_MIN_FILE_LEVEL=IMOL_LOG_LEVEL_WARNING
}

# imol dependencies
include(../../qmake/imol_config.pri)
include(../../qmake/imol_program.pri)

INCLUDEPATH += $$TOP_SRCDIR/include/

LIBS += -limolCore -limolBwaf

# util projects
include(util/QHotkey/qhotkey.pri)

win32 {
SOURCES += util/StackWalker/BaseException.cpp \
        util/StackWalker/StackWalker.cpp

HEADERS += util/StackWalker/BaseException.h \
        util/StackWalker/StackWalker.h \
}

# src files
SOURCES += main.cpp \
    imol.cpp \
    config.cpp \
    license.cpp \
    terminal.cpp \
    server.cpp \

HEADERS += imol.h \
    config.h \
    license.h \
    terminal.h \
    server.h

FORMS += imol.ui \
    terminal.ui

CONFIG += resources_big

RESOURCES += imol_asset.qrc

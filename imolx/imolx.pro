QT += widgets network

TEMPLATE = lib
DEFINES += IMOLX_LIBRARY

CONFIG += c++11

win32 {
QMAKE_CXXFLAGS += /MP /FS
} else {
QMAKE_CXXFLAGS += -std=c++0x
}

DESTDIR = ../

#user defines
DEFINES += MODULE_NAME=\\\"imolx\\\"
CONFIG(debug, debug|release) {
DEFINES += LOG_ENABLE_DETAILS
} else {
DEFINES += LOG_MIN_FILE_LEVEL=IMOL_LOG_LEVEL_INFO
}
#end of user defines

INCLUDEPATH += ../imol/include/ \
    $$PWD/dependencies/include/

LIBS += -L../ -limolCore -limolBwaf

INCLUDEPATH += include/imol/

SOURCES += \
    src/imolx.cpp \
    src/support/basesupporter.cpp \
    src/support/formatter.cpp \
    src/support/lircosformatter.cpp \
    src/support/lircosgrammar.cpp \
    src/support/lircossupporter.cpp \
    src/support/mobjhandler.cpp \
    src/support/xcoresupporter.cpp \
    src/xhelper.cpp \
    src/xmenu.cpp \
    src/xnav.cpp \
    src/xnode.cpp \
    src/xservice.cpp \
    src/xsupport.cpp \
    src/xtheme.cpp

HEADERS += \
    include/imol/imolx_global.h \
    include/imol/mobjhandler.h \
    include/imol/xhelper.h \
    include/imol/xnode.h \
    include/imol/xservice.h \
    src/support/basesupporter.h \
    src/support/formatter.h \
    src/support/lircos/HMICommonDefine.h \
    src/support/lircos/HMICommonStruct.h \
    src/support/lircos/editor_define.h \
    src/support/lircos/editor_struct.h \
    src/support/lircosformatter.h \
    src/support/lircosgrammar.h \
    src/support/lircossupporter.h \
    src/support/xcoresupporter.h \
    src/xmenu.h \
    src/xnav.h \
    src/xtheme.h \
    src/xsupport.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    imolx_asset.qrc

FORMS += \
    src/xmenu.ui

APP_NAME = SplitUI

CONFIG += qt warn_on cascades10
LIBS += -lbbcascadesmultimedia -lbbmultimedia -lbbsystem
LIBS += -lQtXml -lbbsystem -lbbnetwork -lbbdata -lbbdevice -lQtLocationSubset -lcurl -L../libs/cloudbase-libs -lCBHelper
LIBS += -lbbdevice

LIBS += -L ../libs/

LIBS += -llept -ltesseract

LIBS += -lexif

INCLUDEPATH += ../libs/include/

//PRE_TARGETDEPS += ../libs/liblept.so
//PRE_TARGETDEPS += ../libs/libtesseract.so

include(config.pri)

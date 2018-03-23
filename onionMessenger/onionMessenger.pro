TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lpthread
LIBS += -lncurses
OBJECTS_DIR = .obj
MOC_DIR = .MOC

SOURCES += main.cpp \
    message.cpp \
    pgpcrypto.cpp \
    keymanager.cpp \
    messenger.cpp \
    common.cpp \
    sockthread.cpp \
    onionui.cpp

HEADERS += \
    message.h \
    pgpcrypto.h \
    common.h \
    keymanager.h \
    messenger.h \
    sockthread.h \
    json.hpp \
    onionui.h

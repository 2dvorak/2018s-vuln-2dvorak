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
    keymanager.cpp \
    messenger.cpp \
    common.cpp \
    sockthread.cpp \
    onionui.cpp \
    pgpmanager.cpp

HEADERS += \
    message.h \
    common.h \
    keymanager.h \
    messenger.h \
    sockthread.h \
    json.hpp \
    onionui.h \
    pgpmanager.h \
    pgpmanager.h

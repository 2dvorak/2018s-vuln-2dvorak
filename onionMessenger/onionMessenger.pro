TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += console
CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11
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

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lpthread

SOURCES += main.cpp \
    message.cpp \
    pgpcrypto.cpp \
    keymanager.cpp \
    messenger.cpp \
    common.cpp \
    sockthread.cpp

HEADERS += \
    message.h \
    pgpcrypto.h \
    common.h \
    keymanager.h \
    messenger.h \
    sockthread.h \
    json.hpp

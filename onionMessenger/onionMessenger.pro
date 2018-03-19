TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    message.cpp \
    pgpcrypto.cpp \
    newproto.cpp \
    keymanager.cpp \
    messenger.cpp

HEADERS += \
    message.h \
    pgpcrypto.h \
    common.h \
    newproto.h \
    keymanager.h \
    messenger.h

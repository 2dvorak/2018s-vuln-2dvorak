TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    message.cpp \
    pgpcrypto.cpp \
    keymanager.cpp \
    messenger.cpp \
    common.cpp

HEADERS += \
    message.h \
    pgpcrypto.h \
    common.h \
    keymanager.h \
    messenger.h

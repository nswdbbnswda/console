QT += core
QT -= gui
QT += network
CONFIG += c++11
TARGET = console
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    client.cpp \
    iter.cpp \
    mytcpsever.cpp \
    cmdbase.cpp \
    mythread.cpp \
    qcommandlineparser.cpp

HEADERS += \
    server.h \
    client.h \
    iter.h \
    mytcpsever.h \
    cmdbase.h \
    mythread.h \
    qcommandlineparser.h

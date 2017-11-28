QT += core
QT -= gui
QT += network
CONFIG += c++11
TARGET = console
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    iter.cpp \
    mytcpsever.cpp \
    cmdbase.cpp \
    mythread.cpp \
    mainwin.cpp

HEADERS += \
    iter.h \
    mytcpsever.h \
    cmdbase.h \
    mythread.h \
    mainwin.h

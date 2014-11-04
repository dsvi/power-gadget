#-------------------------------------------------
#
# Project created by QtCreator 2014-10-07T02:12:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 qwt precompile_header
CONFIG -= clang_pch_style
TARGET = indicator-power-gadget
TEMPLATE = app

include( ../build-common.pri )

CONFIG +=link_pkgconfig
PKGCONFIG = gtk+-2.0 appindicator-0.1

PRECOMPILED_HEADER = precomp.h

SOURCES += main.cpp\
        MainWindow.cpp \
    EnergyInfoProvider.cpp \
    Exception.cpp \
    PowerPlot.cpp \
    PowerMeter.cpp \
    globals.cpp \
    EngFormattedValue.cpp

HEADERS  += MainWindow.h \
    EnergyInfoProvider.h \
    Exception.h \
    AtExit.h \
    PowerPlot.h \
    PowerMeter.h \
    globals.h \
    EngFormattedValue.h

FORMS    += MainWindow.ui \
    PowerPlot.ui

unix:!macx: LIBS += -L$${PWD}/../ -lintel

INCLUDEPATH += $$PWD/../intel
DEPENDPATH += $$PWD/../intel
unix:!macx: PRE_TARGETDEPS += $${PWD}/../libintel.a


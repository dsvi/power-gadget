#-------------------------------------------------
#
# Project created by QtCreator 2014-10-12T22:06:33
#
#-------------------------------------------------

QT       -= core gui

TARGET = intel
TEMPLATE = lib
CONFIG += staticlib

include( ../build-common.pri )


#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}

HEADERS += \
    cpuid.h \
    msr.h \
    rapl.h

SOURCES += \
    cpuid.c \
    msr.c \
    rapl.c


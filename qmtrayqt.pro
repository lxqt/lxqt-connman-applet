#-------------------------------------------------
#
# Project created by QtCreator 2014-10-10T17:10:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qmtrayqt
TEMPLATE = app


SOURCES += main.cpp\
    systemtray.cpp \
    service.cpp \
    manager.cpp

HEADERS  += \
    systemtray.h \
    service.h \
    manager.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

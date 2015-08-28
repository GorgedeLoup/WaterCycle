#-------------------------------------------------
#
# Project created by QtCreator 2015-08-26T20:47:45
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WaterCycle
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    watercycle.cpp

HEADERS  += mainwindow.h \
    watercycle.h \
    macro.h

FORMS    += mainwindow.ui

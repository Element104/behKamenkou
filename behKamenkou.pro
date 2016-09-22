#-------------------------------------------------
#
# Project created by QtCreator 2016-09-18T20:18:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = behKamenkou
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    APIcko.cpp

HEADERS  += MainWindow.h \
    APIcko.h

FORMS    += MainWindow.ui

LIBS += -lQHttpEngine


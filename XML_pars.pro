#-------------------------------------------------
#
# Project created by QtCreator 2015-10-10T21:39:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XML_pars
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    xmlparse.cpp \
    geo.cpp

HEADERS  += mainwindow.h \
    xmlparse.h \
    geo.h

FORMS    += mainwindow.ui

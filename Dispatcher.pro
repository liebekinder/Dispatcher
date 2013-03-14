#-------------------------------------------------
#
# Project created by QtCreator 2013-03-12T12:55:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Dispatcher
TEMPLATE = app


SOURCES += main.cpp\
    controler.cpp \
    window.cpp \
    server.cpp \
    connection.cpp \
    client.cpp

HEADERS  += \
    controler.h \
    window.h \
    server.h \
    Parameters.h \
    connection.h \
    client.h

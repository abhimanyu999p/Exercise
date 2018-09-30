#-------------------------------------------------
#
# Project created by QtCreator 2018-08-15T12:14:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xLoader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    modem.cpp \
    serial.cpp \
    loginwizard.cpp

HEADERS  += mainwindow.h \
    modem.h \
    serial.h \
    loginwizard.h

FORMS    += mainwindow.ui \
    loginwizard.ui

RESOURCES += \
    myresource.qrc

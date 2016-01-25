#-------------------------------------------------
#
# Project created by QtCreator 2016-01-14T17:23:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_one
TEMPLATE = app
INCLUDEPATH += /usr/include
LIBS += -L/usr/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    saitama.jpg


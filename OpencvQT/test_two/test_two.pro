QT += core
QT -= gui

CONFIG += c++11

TARGET = test_two
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += /usr/include
LIBS += -L/usr/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui


SOURCES += main.cpp




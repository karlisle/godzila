#-------------------------------------------------
#
# Project created by QtCreator 2016-01-20T15:33:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = eyeGaze
TEMPLATE = app
INCLUDEPATH += /usr/include
LIBS += -L/usr/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui
INCLUDEPATH += /intraface

SOURCES += main.cpp\
        mainwindow.cpp \
    eyegaze.cpp \
    adaptivecanny.cpp \
    newhough.cpp \
    trainer.cpp

HEADERS  += mainwindow.h \
    eyegaze.h \
    adaptivecanny.h \
    newhough.h \
    trainer.h



FORMS    += mainwindow.ui

OTHER_FILES += \
    ~/images/cv_lab.jpg

RESOURCES += \
    images/images.qrc


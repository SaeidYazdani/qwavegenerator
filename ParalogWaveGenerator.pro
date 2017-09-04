#-------------------------------------------------
#
# Project created by QtCreator 2017-08-28T13:41:02
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ParalogWaveGenerator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        wave.cpp \
    sinesettingswidget.cpp \
    mysliderwidget.cpp \
    advancedsettingswidget.cpp \
    savewavedialog.cpp

HEADERS += \
        mainwindow.h \
        wave.h \
    sinesettingswidget.h \
    mysliderwidget.h \
    advancedsettingswidget.h \
    savewavedialog.h

FORMS += \
        mainwindow.ui \
    sinesettingswidget.ui \
    mysliderwidget.ui \
    advancedsettingswidget.ui \
    savewavedialog.ui

#-------------------------------------------------
#
# Project created by QtCreator 2014-01-11T14:52:19
#
#-------------------------------------------------

QT       += core gui opengl xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = editor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mappainter.cpp

HEADERS  += mainwindow.h \
    mappainter.h

LIBS += -L/usr/local/lib -lGLU
QMAKE_CXXFLAGS += -O2 -std=c++11 -g

RESOURCES += \
    editor.qrc

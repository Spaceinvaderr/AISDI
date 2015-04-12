#-------------------------------------------------
#
# Project created by QtCreator 2013-12-01T13:11:46
#
#-------------------------------------------------

QT       += core gui opengl xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graphics
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        ../engine/engine.cpp \
        ../engine/junction.cpp \
        ../engine/track.cpp \
        ../engine/train.cpp \
        ../engine/segment.cpp \
	../engine/tileInterpreter.cpp \
	../engine/semafor.cpp \
	decodexml.cpp \
    tileglwidget.cpp \
    parsertilemap.cpp \
    informationwidget.cpp

HEADERS  += mainwindow.h \
        ../engine/engine.hpp \
        decodexml.h \
    tileglwidget.h \
    informationwidget.h

RESOURCES += \
        graphics.qrc

LIBS += -L/usr/local/lib -lGLU
QMAKE_CXXFLAGS += -O2 -std=c++11 -g


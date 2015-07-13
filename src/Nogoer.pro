#-------------------------------------------------
#
# Project created by QtCreator 2015-07-11T11:26:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = Nogoer
TEMPLATE = app

HEADERS += \
    mainwidget.h \
    chessengine.h \
    chessscript.h \
    luascript.h \
    chessboard.h \
    commondef.h \
    chessboardwidget.h \
    clockwidget.h \
    historywidget.h \
    historyitem.h \
    historylist.h \
    controlwidget.h \
    showlogwidget.h

SOURCES += \
    main.cpp \
    mainwidget.cpp \
    chessengine.cpp \
    chessscript.cpp \
    luascript.cpp \
    chessboard.cpp \
    chessboardwidget.cpp \
    clockwidget.cpp \
    historywidget.cpp \
    historyitem.cpp \
    historylist.cpp \
    controlwidget.cpp \
    showlogwidget.cpp

LIBS += $$PWD/lua/lua.a

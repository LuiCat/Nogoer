#-------------------------------------------------
#
# Project created by QtCreator 2015-07-11T11:26:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Nogoer
TEMPLATE = app

HEADERS += \
    mainwidget.h \
    chessengine.h \
    chessscript.h \
    luascript.h \
    chessboard.h \
    commondef.h

SOURCES += \
    main.cpp \
    mainwidget.cpp \
    chessengine.cpp \
    chessscript.cpp \
    luascript.cpp \
    chessboard.cpp

LIBS += $$PWD/lua/lua.a

#-------------------------------------------------
#
# Project created by QtCreator 2012-05-29T20:01:38
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = SocksServerConsole
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += .

SOURCES += main.cpp

#Linkage for SocksLib shared lib
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../SocksLib/release/ -lSocksLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../SocksLib/debug/ -lSocksLib
else:unix:!symbian: LIBS += -L$$OUT_PWD/../SocksLib/ -lSocksLib

INCLUDEPATH += $$PWD/../SocksLib
DEPENDPATH += $$PWD/../SocksLib

#-------------------------------------------------
#
# Project created by QtCreator 2012-05-29T20:15:32
#
#-------------------------------------------------

QT       += network testlib

QT       -= gui

TARGET = tst_BasicTCPTests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += .

SOURCES += tst_BasicTCPTests.cpp \
    TCPTestServer.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    TCPTestServer.h

#SocksLib linkage
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../SocksLib/release/ -lSocksLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../SocksLib/debug/ -lSocksLib
else:unix:!symbian: LIBS += -L$$OUT_PWD/../SocksLib/ -lSocksLib

INCLUDEPATH += $$PWD/../SocksLib
DEPENDPATH += $$PWD/../SocksLib

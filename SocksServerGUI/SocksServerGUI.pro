#-------------------------------------------------
#
# Project created by QtCreator 2012-05-29T20:07:37
#
#-------------------------------------------------

QT       += core gui

TARGET = SocksServerGUI
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui

#Linkage for SocksLib
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../SocksLib/release/ -lSocksLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../SocksLib/debug/ -lSocksLib
else:unix:!symbian: LIBS += -L$$OUT_PWD/../SocksLib/ -lSocksLib

INCLUDEPATH += $$PWD/../SocksLib
DEPENDPATH += $$PWD/../SocksLib

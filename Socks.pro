#-------------------------------------------------
#
# Project created by QtCreator 2012-03-15T14:52:14
#
#-------------------------------------------------

QT       += core gui network

TARGET = Socks
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    SocksServer.cpp \
    SocksConnection.cpp \
    states/InitialState.cpp \
    protocol/SocksProtocolMessage.cpp \
    protocol/Socks5RequestMessage.cpp \
    protocol/Socks5ReplyMessage.cpp \
    protocol/SocksReplyMessage4a.cpp \
    states/SocksState.cpp \
    protocol/Socks5GreetingMessage.cpp \
    states/Socks5AuthState.cpp \
    protocol/Socks5MethodSelectionMessage.cpp \
    states/Socks5AuthenticatedState.cpp \
    states/Socks5ConnectState.cpp \
    states/Socks5ConnectedState.cpp \
    states/Socks4InitialState.cpp \
    protocol/Socks4RequestMessage.cpp \
    states/Socks4ConnectState.cpp \
    protocol/Socks4ReplyMessage.cpp \
    states/Socks4ConnectedState.cpp \
    states/Socks5UDPAssociateState.cpp \
    states/Socks5UDPAssociatedState.cpp \
    protocol/Socks5UDPRequestMessage.cpp \
    decorators/QIODeviceDecorator.cpp

HEADERS  += MainWindow.h \
    SocksServer.h \
    SocksConnection.h \
    states/InitialState.h \
    protocol/SocksProtocolMessage.h \
    protocol/Socks5RequestMessage.h \
    protocol/Socks5ReplyMessage.h \
    protocol/SocksReplyMessage4a.h \
    states/SocksState.h \
    protocol/Socks5GreetingMessage.h \
    states/Socks5AuthState.h \
    protocol/Socks5MethodSelectionMessage.h \
    states/Socks5AuthenticatedState.h \
    states/Socks5ConnectState.h \
    states/Socks5ConnectedState.h \
    states/Socks4InitialState.h \
    protocol/Socks4RequestMessage.h \
    states/Socks4ConnectState.h \
    protocol/Socks4ReplyMessage.h \
    states/Socks4ConnectedState.h \
    states/Socks5UDPAssociateState.h \
    states/Socks5UDPAssociatedState.h \
    protocol/Socks5UDPRequestMessage.h \
    decorators/QIODeviceDecorator.h

FORMS    += MainWindow.ui

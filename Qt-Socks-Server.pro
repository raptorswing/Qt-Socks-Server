TEMPLATE = subdirs

SUBDIRS += SocksLib \
    SocksServerConsole \
    SocksServerGUI \
    Tests

SocksServerConsole.depends += SocksLib

SocksServerGUI.depends += SocksLib

Tests.depends += SocksLib

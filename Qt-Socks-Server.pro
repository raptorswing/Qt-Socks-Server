TEMPLATE = subdirs

SUBDIRS += SocksLib \
    SocksServerConsole \
    SocksServerGUI

SocksServerConsole.depends += SocksLib

SocksServerGUI.depends += SocksLib

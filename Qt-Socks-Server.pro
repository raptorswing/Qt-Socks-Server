TEMPLATE = subdirs

SUBDIRS += SocksLib \
    SocksServerConsole

SocksServerConsole.depends += SocksLib

TARGET = appmenu-qt5

PLUGIN_TYPE = platformthemes
PLUGIN_CLASS_NAME = AppMenuPlatformThemePlugin
load(qt_plugin)

QT += core-private gui-private platformsupport-private dbus widgets
DBUS_INTERFACES += com.canonical.AppMenu.Registrar.xml

CONFIG += X11 link_pkgconfig debug
PKGCONFIG += dbusmenu-qt5 gtk+-2.0
DESTDIR = ./

HEADERS += \
        appmenuplatformmenuitem.h \
        appmenuplatformmenu.h \
        appmenuplatformmenubar.h

SOURCES += \
        appmenuplatformmenuitem.cpp \
        appmenuplatformmenu.cpp \
        appmenuplatformmenubar.cpp


TARGET = appmenu-qt5

PLUGIN_TYPE = platformthemes
PLUGIN_CLASS_NAME = AppMenuPlatformThemePlugin
load(qt_plugin)

QT += core-private gui-private platformsupport-private dbus widgets
DBUS_INTERFACES += com.canonical.AppMenu.Registrar.xml

DEFINES += QKDETHEME_STILL_PRIVATE

CONFIG += X11 link_pkgconfig debug
PKGCONFIG += dbusmenu-qt5
DESTDIR = ./

HEADERS += \
        appmenuplatformmenubar.h

SOURCES += \
        appmenuplatformmenubar.cpp


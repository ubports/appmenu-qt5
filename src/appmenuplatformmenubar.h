#ifndef APPMENUPLATFORMMENUBAR_H
#define APPMENUPLATFORMMENUBAR_H

#include <QtGui/5.1.1/QtGui/qpa/qplatformmenu.h>
#include <QtGui/5.1.1/QtGui/qpa/qplatformtheme.h>
#include <QtGui/5.1.1/QtGui/qpa/qplatformthemeplugin.h>

class QMenuBar;
class QWindow;
class QString;

class MenuBarAdapter;

class AppMenuPlatformMenuBar : public QPlatformMenuBar
{
    Q_OBJECT
public:
    AppMenuPlatformMenuBar();
    virtual ~AppMenuPlatformMenuBar();

    virtual void insertMenu(QPlatformMenu *menu, QPlatformMenu* before);
    virtual void removeMenu(QPlatformMenu *menu);
    virtual void syncMenu(QPlatformMenu *menuItem);
    virtual void handleReparent(QWindow *newParentWindow);
    virtual QPlatformMenu *menuForTag(quintptr tag) const;

private:

    QMenuBar *m_menubar;
    QWindow *m_window;
    MenuBarAdapter *m_adapter;
    QString m_objectPath;
};


class AppMenuPlatformThemePlugin : public QPlatformThemePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPA.QPlatformThemeFactoryInterface.5.1" FILE "appmenu-qt5.json")
public:
    AppMenuPlatformThemePlugin(QObject *parent = 0);

    virtual QPlatformTheme *create(const QString &key, const QStringList &paramList);
};

QT_END_NAMESPACE

#endif

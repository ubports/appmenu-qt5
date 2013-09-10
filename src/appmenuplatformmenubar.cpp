#include "appmenuplatformmenubar.h"
#include "registrar_interface.h"

#include <dbusmenuexporter.h>

#include <QWindow>
#include <QWidget>
#include <QString>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QDebug>

#define LOG qDebug() << "appmenu-qt:" << __FUNCTION__ << __LINE__
#define LOG_VAR(x) qDebug() << "appmenu-qt:" << __FUNCTION__ << __LINE__ << #x ":" << x
#define WARN qWarning() << "appmenu-qt:" << __FUNCTION__ << __LINE__

QT_BEGIN_NAMESPACE

static const char* REGISTRAR_SERVICE = "com.canonical.AppMenu.Registrar";
static const char* REGISTRAR_PATH    = "/com/canonical/AppMenu/Registrar";
static const char* REGISTRAR_IFACE   = "com.canonical.AppMenu.Registrar";


/*
 * The menubar adapter communicates over DBus with the menubar renderer.
 * It is responsible for registering windows to it and exposing their menubars
 * if they have one.
 */
class MenuBarAdapter
{
public:
    MenuBarAdapter(QMenuBar*, const QString&);
    ~MenuBarAdapter();

    bool registerWindow();
    void popupAction(QAction*);
    void setAltPressed(bool pressed);
    void resetRegisteredWinId();

private:
    uint m_registeredWinId;
    DBusMenuExporter* m_exporter;
    QMenuBar* m_menuBar;
    QString m_objectPath;
};





///////////////////////////////////////////////////////////
AppMenuPlatformMenuBar::AppMenuPlatformMenuBar()
    : QPlatformMenuBar(),
      m_menubar(0),
      m_window(0),
      m_adapter(0)
{
}

AppMenuPlatformMenuBar::~AppMenuPlatformMenuBar()
{
}

void 
AppMenuPlatformMenuBar::insertMenu(QPlatformMenu *menu, QPlatformMenu *before)
{
    return;
}

void 
AppMenuPlatformMenuBar::removeMenu(QPlatformMenu *menu)
{
    return;
}

void 
AppMenuPlatformMenuBar::syncMenu(QPlatformMenu *menuItem)
{
    return;
}

void 
AppMenuPlatformMenuBar::handleReparent(QWindow *newParentWindow)
{
    if (!newParentWindow)
        return;

    static int menuBarId = 1;
    m_objectPath = QString(QLatin1String("/MenuBar/%1")).arg(menuBarId++);

    m_window = newParentWindow;
    QWidget *window = QWidget::find(m_window->winId());
    m_menubar = window->findChild<QMenuBar *>();
    if (!m_menubar) {
        // Something went wrong, this shouldn't have happened
        WARN << "The given QWindow has no QMenuBar assigned";
        return;
    }

    delete m_adapter;
    m_adapter = new MenuBarAdapter(m_menubar, m_objectPath);
    m_adapter->registerWindow();
}

QPlatformMenu *
AppMenuPlatformMenuBar::menuForTag(quintptr tag) const
{
    return NULL;
}


///////////////////////////////////////////////////////////
MenuBarAdapter::MenuBarAdapter(QMenuBar* _menuBar, const QString& _objectPath)
    : m_registeredWinId(0),
      m_exporter(0), 
      m_menuBar(_menuBar),
      m_objectPath(_objectPath)
{
}

MenuBarAdapter::~MenuBarAdapter()
{
    delete m_exporter;
    m_exporter = 0;
}

bool
MenuBarAdapter::registerWindow()
{
    static com::canonical::AppMenu::Registrar *registrar = 0;

    if (!m_menuBar->window()) {
        WARN << "No parent for this menubar";
        return false;
    }

    uint winId = m_menuBar->window()->winId();
    if (winId == m_registeredWinId)
        return true;

    if (!registrar)
        registrar = new com::canonical::AppMenu::Registrar(REGISTRAR_SERVICE, REGISTRAR_PATH, QDBusConnection::sessionBus(), 0);

    if (!registrar || !registrar->isValid())
        return false;

    if (!m_exporter)
        m_exporter = new DBusMenuExporter(m_objectPath, (QMenu *)m_menuBar);

    m_registeredWinId = winId;

    registrar->RegisterWindow(winId, QDBusObjectPath(m_objectPath));

    return true;
}

void
MenuBarAdapter::resetRegisteredWinId()
{
    m_registeredWinId = 0;
}

void
MenuBarAdapter::popupAction(QAction* action)
{
    m_exporter->activateAction(action);
}

void
MenuBarAdapter::setAltPressed(bool pressed)
{
    // m_exporter may be 0 if the menubar is empty.
    if (m_exporter) {
        m_exporter->setStatus(pressed ? "notice" : "normal");
    }
}


///////////////////////////////////////////////////////////
class AppMenuPlatformTheme : public QPlatformTheme
{
public:
    virtual QPlatformMenuItem* createPlatformMenuItem() const;
    virtual QPlatformMenu* createPlatformMenu() const;
    virtual QPlatformMenuBar* createPlatformMenuBar() const;
};


QPlatformMenuItem *
AppMenuPlatformTheme::createPlatformMenuItem() const
{
    return 0;
}

QPlatformMenu *
AppMenuPlatformTheme::createPlatformMenu() const
{
    return 0;
}

QPlatformMenuBar *
AppMenuPlatformTheme::createPlatformMenuBar() const
{
    return new AppMenuPlatformMenuBar();
}


///////////////////////////////////////////////////////////
AppMenuPlatformThemePlugin::AppMenuPlatformThemePlugin(QObject *parent)
{
}

QPlatformTheme *
AppMenuPlatformThemePlugin::create(const QString &key, const QStringList &paramList)
{
    return new AppMenuPlatformTheme();
}


///////////////////////////////////////////////////////////

QT_END_NAMESPACE

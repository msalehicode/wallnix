#include <QApplication>              // Use QApplication, not QGuiApplication
#include <QQmlApplicationEngine>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QQuickWindow>
#include <QCursor>
#include <QQmlContext>


#include "wallpapermanager.h"
// #include "wallpapermanger_particle_example.h"
// #include "wallpapermanager_shapedrawAndmove_example.h"
// #include "wallpapermanager_shader_example.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    WallpaperManager wallpaperManager(&app);
    // Expose wallpaperManager to QML if needed
    engine.rootContext()->setContextProperty("wallpaperManager", &wallpaperManager);



    const QUrl url(QStringLiteral("qrc:/wallnix/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);

    engine.load(url);



    if (engine.rootObjects().isEmpty())
        return -1;

    // Get QQuickWindow from root QML object
    QQuickWindow *window = qobject_cast<QQuickWindow*>(engine.rootObjects().first());
    if (!window) {
        qWarning("Faileds to get QQuickWindow from QML root object");
        return -1;
    }

    // Setup system tray icon
    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        qWarning("System tray is not available on this system");
        return -1;
    }

    QSystemTrayIcon trayIcon;
    trayIcon.setIcon(QIcon("./resources/tray.svg")); // change to your icon path
    trayIcon.setToolTip("Wallnix Live Wallpaper");

    QMenu menu;
    QAction openAction("Open Wallnix Window");
    QAction startAction("Resume Wallpaper");
    QAction stopAction("Pause Wallpaper");
    QAction quitAction("Quit");
    menu.addAction(&openAction);
    menu.addSeparator();

    menu.addAction(&startAction);
    menu.addAction(&stopAction);
    menu.addSeparator();

    menu.addAction(&quitAction);

    trayIcon.setContextMenu(&menu);

    // Show or hide window when clicking tray icon or menu action
    QObject::connect(&openAction, &QAction::triggered, [window]() {
        if (window->isVisible())
            window->hide();
        else {
            window->show();
            window->raise();
            window->requestActivate();
        }
    });

    QObject::connect(&quitAction, &QAction::triggered, &app, &QApplication::quit);
    QObject::connect(&startAction, &QAction::triggered, &wallpaperManager, &WallpaperManager::startWallpaper);
    QObject::connect(&stopAction, &QAction::triggered, &wallpaperManager, &WallpaperManager::stopWallpaper);

    QObject::connect(&trayIcon, &QSystemTrayIcon::activated,
                     [window, &menu](QSystemTrayIcon::ActivationReason reason) {
                         if (reason == QSystemTrayIcon::Trigger) { // Left-click
                             if (window->isVisible())
                                 window->hide();
                             else {
                                 window->show();
                                 window->raise();
                                 window->requestActivate();
                             }
                         } else if (reason == QSystemTrayIcon::Context) { // Right-click
                             menu.exec(QCursor::pos());
                         }
                     });

    trayIcon.show();

    // Optionally start window hidden
    // window->hide();

    return app.exec();
}

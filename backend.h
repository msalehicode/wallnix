#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>


#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

#include <QQuickWindow>


#include "autostartmanager.h"
#include "settingsmanager.h"
#include "wallpapermanager.h"



class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);

    //init functions to call by main
    bool setWindow(QQuickWindow *window, QApplication* app);
    bool setupTrayIcon();


    //qml
    Q_INVOKABLE void startWallpaperbyQML();
    Q_INVOKABLE void stopWallpaperbyQML();
    Q_INVOKABLE void changeWallpaperbyQML(const QString &videoPath);
    Q_INVOKABLE void getCurrentWallpaper();

    Q_INVOKABLE void setSettingValue(const QString& key, const QVariant& value);
    Q_INVOKABLE QVariant getSettingValue(const QString& key);

    Q_INVOKABLE void setRunOnStartUp(const QString& status);
    Q_INVOKABLE void setAutoPause(const QString& status);

private:
    WallpaperManager wallpaperManager;
    SettingsManager settingsManager;
    AutostartManager autoStartManager;

    QQuickWindow* m_window;
    QApplication* m_app;
    QSystemTrayIcon m_trayIcon;
    QMenu m_menu;

signals:
    void currentWallpaperIs(const QString& currentWPath);
    void resultChangeWallpaperPath(const QString& res);
};

#endif // BACKEND_H

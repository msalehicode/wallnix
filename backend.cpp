#include "backend.h"

Backend::Backend(QObject *parent)
    : QObject{parent}
{
    //make sure settings key,values are set if does not exist
    settingsManager.initSettings();

    //load currentWallpaperPath from QSetting
    QString path = settingsManager.getValue("currentWallpaperPath").toString();
    if(wallpaperManager.setCurrentWallpaperPath(path)!="ok")
    {
        wallpaperManager.setCurrentWallpaperPath("/usr/share/wallnix/resources/default-wallpaper.gif");
        qInfo() << "could not fetched wallpaper from QSetting into wallpaper, default wallpaper replaced";
    }

}

bool Backend::setWindow(QQuickWindow* window, QApplication* app)
{
    if (!window)
    {
        qWarning("Faileds to get QQuickWindow from QML root object");
        return false;
    }
    m_window = window;
    m_app = app;

    initApp();
    return true;
}

bool Backend::setupTrayIcon()
{

    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        qWarning("System tray is not available on this system");
        return false;
    }

    m_trayIcon.setIcon(QIcon("/usr/share/wallnix/resources/tray.svg"));
    m_trayIcon.setToolTip("Wallnix Live Wallpaper");

    QAction* openAction = new QAction("Open Wallnix Window", &m_menu);
    QAction* startAction = new QAction("Resume Wallpaper", &m_menu);
    QAction* stopAction = new QAction("Pause Wallpaper", &m_menu);
    QAction* quitAction = new QAction("Quit", &m_menu);

    m_menu.addAction(openAction);
    m_menu.addSeparator();
    m_menu.addAction(startAction);
    m_menu.addAction(stopAction);
    m_menu.addSeparator();
    m_menu.addAction(quitAction);

    m_trayIcon.setContextMenu(&m_menu);


    // Show or hide window when clicking tray icon or menu action
    QObject::connect(openAction, &QAction::triggered, [this]()
                     {
                         if (m_window->isVisible())
                             m_window->hide();
                         else
                         {
                             m_window->show();
                             m_window->raise();
                             m_window->requestActivate();
                         }
                     });

    QObject::connect(quitAction, &QAction::triggered, m_app, &QApplication::quit);
    QObject::connect(startAction, &QAction::triggered, &wallpaperManager, &WallpaperManager::startWallpaper);
    QObject::connect(stopAction, &QAction::triggered, &wallpaperManager, &WallpaperManager::stopWallpaper);

    QObject::connect(&m_trayIcon, &QSystemTrayIcon::activated,
                     [this](QSystemTrayIcon::ActivationReason reason)
                     {
                         if (reason == QSystemTrayIcon::Trigger)
                         { // Left-click
                             if (m_window->isVisible())
                                 m_window->hide();
                             else
                             {
                                 m_window->show();
                                 m_window->raise();
                                 m_window->requestActivate();
                             }
                         }
                         else if (reason == QSystemTrayIcon::Context)
                         { // Right-click
                             m_menu.exec(QCursor::pos());
                         }
                     });

    m_trayIcon.show();


    // Optionally start window hidden
    // m_window->hide();

    return true;
}

void Backend::startWallpaperbyQML()
{
    wallpaperManager.startWallpaper();
}

void Backend::stopWallpaperbyQML()
{
    wallpaperManager.stopWallpaper();
}

void Backend::changeWallpaperbyQML(const QString &videoPath)
{
    QString result= wallpaperManager.setCurrentWallpaperPath(videoPath);
    if(result=="ok")
    {
        wallpaperManager.stopWallpaper();
        wallpaperManager.startWallpaper();
        settingsManager.setValue("currentWallpaperPath", videoPath);
        qInfo() << "wallpaper path changed";
    }
    else
    {
        qInfo() << "could not change wallaper path";
    }
    emit resultChangeWallpaperPath(result);
}

void Backend::getCurrentWallpaper()
{
    emit currentWallpaperIs(wallpaperManager.currentWallpaperPath);
}

void Backend::setSettingValue(const QString &key, const QVariant &value)
{
    settingsManager.setValue(key,value);
}

QVariant Backend::getSettingValue(const QString &key)
{
    return settingsManager.getValue(key);
}

void Backend::setRunOnStartUp(const QString &status)
{
    bool st;
    if(status=="false")
        st = false;
    else
        st = true;
    if(st!=autoStartManager.getStatus())
    {
        if(autoStartManager.setAutostartEnabled(st))
        {
            autoStartManager.setStatus(st);
            settingsManager.setValue("is_startup_enabled",st);
        }
        else
            qInfo() << "failed to set status on auto start.";
    }

}

void Backend::setAutoPause(const QString &status)
{
    //code
}

void Backend::setVideoLoop(const QString &status)
{
    if (status.compare("true", Qt::CaseInsensitive) == 0 || status == "1")
        wallpaperManager.setLoopVideo(true);
    else
        wallpaperManager.setLoopVideo(false);
}

void Backend::setVideoMute(const QString &status)
{
    if (status.compare("true", Qt::CaseInsensitive) == 0 || status == "1")
        wallpaperManager.setMuteVideo(true);
    else
        wallpaperManager.setMuteVideo(false);
}

void Backend::setVideoVolume(const QString &vol)
{
    wallpaperManager.setVideoSoundVolume(vol.toInt());
}

void Backend::setVideoPauseResume()
{
    wallpaperManager.videoPauseOrResume();
}

void Backend::initApp()
{
    wallpaperManager.startWallpaper();

    QThread::sleep(3);
    bool res_trayIcon=false;
    while(res_trayIcon==false)
    {
        res_trayIcon = setupTrayIcon();
        if(res_trayIcon==false)
        {
            qWarning("System tray is not available on this system wait 3s try again...");
            QThread::sleep(3);
        }
    }
}

#ifndef WALLPAPERMANAGER_H
#define WALLPAPERMANAGER_H

#include <QApplication>
#include <QLabel>
#include <QMovie>
#include <QScreen>
#include <QWidget>
#include <QPainter>
#include <QCursor>
#include <QTimer>
#include <QDebug>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <QFile>
#include <QFileInfo>


#include "wallpaperinstance.h"

class WallpaperManager : public QObject {
    Q_OBJECT
public:
    
    
    explicit WallpaperManager(QObject *parent = nullptr);
    QString currentWallpaperPath;
    ~WallpaperManager();

    void startWallpaper();
    void stopWallpaper();
    QString setCurrentWallpaperPath(const QString &newCurrentWallpaperPath);

    void setMuteVideo(bool status);
    void setLoopVideo(bool status);
    void setVideoSoundVolume(int vol);
    void videoPauseOrResume();
private:
    Display* display = nullptr;
    Window desktopWindow;
    QList<QLabel*> labels;
    QList<QMovie*> movies;
    Window findDesktopWindow(Display* display);

    QList<WallpaperInstance*> wallpaperInstances;//for multiple monitor support
};

#endif

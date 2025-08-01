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

class WallpaperManager : public QObject {
    Q_OBJECT
public:
    
    
    explicit WallpaperManager(QObject *parent = nullptr);
    QString currentWallpaperPath;
    ~WallpaperManager();

    void startWallpaper();
    void stopWallpaper();
    QString setCurrentWallpaperPath(const QString &newCurrentWallpaperPath);

private:
    Display* display = nullptr;
    Window desktopWindow;
    QList<QLabel*> labels;
    QList<QMovie*> movies;
    Window findDesktopWindow(Display* display);

};

#endif

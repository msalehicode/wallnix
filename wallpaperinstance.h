#ifndef WALLPAPERINSTANCE_H
#define WALLPAPERINSTANCE_H

#include <QLabel>
#include <QMovie>
#include <QScreen>
#include <X11/Xlib.h>
#include <vlc/vlc.h>
#include <QFile>

struct WallpaperInstance {
    enum class Type { GIF, Video };
    Type type;

    QLabel* gifLabel = nullptr;
    QMovie* gifMovie = nullptr;

    QWidget* videoWidget = nullptr;
    libvlc_instance_t* vlcInstance = nullptr;
    libvlc_media_player_t* vlcPlayer = nullptr;

    libvlc_media_t* vlcMedia = nullptr;


    WallpaperInstance(QScreen *screen, const QString &filePath, Display *display, Window desktopWindow);

    ~WallpaperInstance();
};


#endif // WALLPAPERINSTANCE_H

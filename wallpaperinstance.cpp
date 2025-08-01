#include "wallpaperinstance.h"

WallpaperInstance::WallpaperInstance(QScreen *screen, const QString &filePath, Display *display,  Window desktopWindow)
{
    QRect geo = screen->geometry();

    if (filePath.endsWith(".gif", Qt::CaseInsensitive)) {
        type = Type::GIF;
        gifLabel = new QLabel(nullptr);

        gifLabel->setWindowFlags(Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
        gifLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
        gifLabel->setFocusPolicy(Qt::NoFocus);

        gifLabel->resize(geo.size());
        gifLabel->move(geo.topLeft());

        gifMovie = new QMovie(filePath);
        gifMovie->setScaledSize(geo.size());
        gifLabel->setMovie(gifMovie);

        gifLabel->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        gifLabel->setAttribute(Qt::WA_X11NetWmWindowTypeDesktop);

        gifMovie->start();
        XMapWindow(display, gifLabel->winId());
        gifLabel->show();
    }
    else if (filePath.endsWith(".mp4", Qt::CaseInsensitive) ||
             filePath.endsWith(".avi", Qt::CaseInsensitive) ||
             filePath.endsWith(".mkv", Qt::CaseInsensitive) ||
             filePath.endsWith(".mov", Qt::CaseInsensitive) ||
             filePath.endsWith(".webm", Qt::CaseInsensitive))
    {
        // Video case
        type = Type::Video;

        QWidget* container = new QWidget(nullptr);
        container->setWindowFlags(Qt::FramelessWindowHint);
        container->setAttribute(Qt::WA_TransparentForMouseEvents);
        container->setFocusPolicy(Qt::NoFocus);
        container->resize(geo.size());
        container->move(geo.topLeft());
        container->setAttribute(Qt::WA_X11NetWmWindowTypeDesktop);


        // Set the native parent (this requires using X11 calls)
        Window xid = container->winId();
        Display* dpy = display;


        // XMapWindow(display, container->winId());


        // Reparent the VLC widget (container) to the desktop window
        XReparentWindow(dpy, xid, desktopWindow, geo.x(), geo.y());

        // Map the window after reparenting
        XMapWindow(dpy, xid);

        container->show();

        // VLC setup
        const char* const vlc_args[] = {"--quiet" }; //"--no-xlib"
        vlcInstance = libvlc_new(1, vlc_args);
        if (!vlcInstance) {
            qWarning() << "Failed to create VLC instance.";
            return;
        }
        QByteArray filePathBytes = QFile::encodeName(filePath);
        vlcMedia = libvlc_media_new_path(vlcInstance, filePathBytes.constData());

        // Add loop option here
        libvlc_media_add_option(vlcMedia, ":input-repeat=-1");
        libvlc_media_add_option(vlcMedia, ":repeat");

        vlcPlayer = libvlc_media_player_new_from_media(vlcMedia);
        libvlc_media_player_set_xwindow(vlcPlayer, container->winId());
        libvlc_media_player_play(vlcPlayer);


    }
    else
    {
        qFatal() << "unsupported format.";
    }
}

WallpaperInstance::~WallpaperInstance() {
    if (type == Type::GIF) {
        if (gifMovie) {
            gifMovie->stop();
            delete gifMovie;
        }
        if (gifLabel) {
            gifLabel->hide();
            delete gifLabel;
        }
    } else {
        if (vlcPlayer) {
            libvlc_media_player_stop(vlcPlayer);
            libvlc_media_player_release(vlcPlayer);
        }
        if (vlcMedia) {
            libvlc_media_release(vlcMedia);
        }
        if (vlcInstance) {
            libvlc_release(vlcInstance);
        }
    }
}

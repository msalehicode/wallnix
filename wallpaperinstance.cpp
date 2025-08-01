#include "wallpaperinstance.h"

void WallpaperInstance::loopVideo(bool loop)
{
    isVlcLoopEnabled = loop;
}

void WallpaperInstance::muteVideoSound(bool mute)
{
    isVlcSoundMuted = mute;
    libvlc_audio_set_mute(vlcPlayer, isVlcSoundMuted);
}

void WallpaperInstance::setVideoSoundVolume(int vol)
{
    if(vol >=0 || vol <=100)
    {
        if(isVlcSoundMuted==true)
            muteVideoSound(false);

        videoSoundVolume = vol;
        libvlc_audio_set_volume(vlcPlayer, videoSoundVolume);
    }
    else
    {
        qInfo() << " invalid volume value";
    }
}

void WallpaperInstance::pauseResume()
{
    libvlc_media_player_pause(vlcPlayer);
    // libvlc_media_player_set_pause(vlcPlayer, 1);
    // libvlc_media_player_set_pause(vlcPlayer, 0);
}

WallpaperInstance::WallpaperInstance(QScreen *screen, const QString &filePath, Display *display,  Window desktopWindow)
{
    //set default values but in parent (wallpapaerManager) will set these values from settings/config
    isVlcLoopEnabled=true;
    isVlcSoundMuted=false;
    videoSoundVolume=50;

    QRect geo = screen->geometry();

    if (filePath.endsWith(".gif", Qt::CaseInsensitive)) {
        type = Type::GIF;
        gifLabel = new QLabel(nullptr);

        gifLabel->setWindowFlags(Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
        gifLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
        gifLabel->setFocusPolicy(Qt::NoFocus);
        gifLabel->setAttribute(Qt::WA_Disabled);

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
        container->setAttribute(Qt::WA_Disabled);
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
        const char* const vlc_args[] = {"--quiet"}; //"--no-xlib"
        vlcInstance = libvlc_new(1, vlc_args);
        if (!vlcInstance) {
            qWarning() << "Failed to create VLC instance.";
            return;
        }

        QByteArray filePathBytes = QFile::encodeName(filePath);
        vlcMedia = libvlc_media_new_path(vlcInstance, filePathBytes.constData());

        vlcPlayer = libvlc_media_player_new_from_media(vlcMedia);

        libvlc_event_manager_t* eventManager = libvlc_media_player_event_manager(vlcPlayer);
        libvlc_event_attach(eventManager, libvlc_MediaPlayerEndReached,
                            [](const libvlc_event_t* event, void* userData) {
                                WallpaperInstance* instance = static_cast<WallpaperInstance*>(userData);
                                if (instance && instance->isVlcLoopEnabled) {
                                    // Post to Qt main thread with a slight delay
                                    QMetaObject::invokeMethod(QCoreApplication::instance(), [instance]() {
                                        libvlc_media_player_stop(instance->vlcPlayer);
                                        // libvlc_media_player_set_media(instance->vlcPlayer, instance->vlcMedia);
                                        libvlc_media_player_play(instance->vlcPlayer);
                                        qInfo() << "Restarted video on main thread";
                                    }, Qt::QueuedConnection);
                                }
                            }, this);



        // make vlc loop second approach to do loop, but its not smooth n optimize
        // loopTimer = new QTimer();
        // loopTimer->setInterval(0);  // check every 1 second
        // QObject::connect(loopTimer, &QTimer::timeout, [this]() {
        //     if (vlcPlayer && libvlc_media_player_get_state(vlcPlayer) == libvlc_Ended)
        //     {
        //         libvlc_media_player_stop(vlcPlayer);               // ✅ reset internal state
        //         libvlc_media_player_set_media(vlcPlayer, vlcMedia); // ⬅ required if media is lost
        //         libvlc_media_player_play(vlcPlayer);               // ✅ restart
        //         qInfo() << "qtimer found video is eneded";
        //     }
        // });
        // loopTimer->start();
        qInfo() << "Using libVLC version:" << libvlc_get_version();



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
        if (loopTimer) {
            loopTimer->stop();
            delete loopTimer;
        }
    }
}

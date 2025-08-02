#include "wallpapermanager.h"

WallpaperManager::WallpaperManager(QObject *parent) : QObject(parent)
{
    display = XOpenDisplay(nullptr);
    if (!display)
    {
        qFatal("Failed to open X11 display");
    }
    desktopWindow = findDesktopWindow(display);
}


WallpaperManager::~WallpaperManager()
{
    for (auto label : labels) {
        label->deleteLater();
    }
    if (display) {
        XCloseDisplay(display);
    }
}

void WallpaperManager::startWallpaper() {
    if (!display) return;
    if (!wallpaperInstances.isEmpty()) {
        qWarning() << "Wallpaper already running, stop first!";
        return;
    }

    qDebug() << "desktopWindow =" << desktopWindow;
    if (desktopWindow == 0) {
        desktopWindow = DefaultRootWindow(display);
    }


    const auto screens = QApplication::screens();
    for (QScreen* screen : screens) {
        WallpaperInstance* instance = new WallpaperInstance(screen, currentWallpaperPath, display, desktopWindow);
        wallpaperInstances.append(instance);
    }
    XFlush(display);
}


void WallpaperManager::stopWallpaper()
{
    qDeleteAll(wallpaperInstances);
    wallpaperInstances.clear();
    if (display)
        XFlush(display);
}


QString WallpaperManager::setCurrentWallpaperPath(const QString &newCurrentWallpaperPath)
{
    QString res;

    // List of supported video extensions (lowercase)
    const QStringList videoExtensions = {".mp4", ".avi", ".mkv", ".mov", ".webm"};

    // Convert file extension to lowercase for case-insensitive comparison
    QString ext = QFileInfo(newCurrentWallpaperPath).suffix().toLower();

    if (ext == "gif" || videoExtensions.contains("." + ext))
    {
        if (QFile::exists(newCurrentWallpaperPath))
        {
            currentWallpaperPath = newCurrentWallpaperPath;
            res = "ok";
        }
        else
        {
            qWarning() << "File does not exist:" << newCurrentWallpaperPath;
            res = "file does not exist.";
        }
    }
    else
    {
        res = "file format not supported.";
        qWarning() << "File format not supported:" << newCurrentWallpaperPath;
    }

    return res;
}

void WallpaperManager::setMuteVideo(bool status)
{
    for (WallpaperInstance* instance : wallpaperInstances) {
        if (instance && instance->type == WallpaperInstance::Type::Video) {
            instance->muteVideoSound(status);
        }
    }
}
void WallpaperManager::setLoopVideo(bool status)
{
    for (WallpaperInstance* instance : wallpaperInstances) {
        if (instance && instance->type == WallpaperInstance::Type::Video) {
            instance->loopVideo(status);
        }
    }
}

void WallpaperManager::setVideoSoundVolume(int vol)
{
    for (WallpaperInstance* instance : wallpaperInstances) {
        if (instance && instance->type == WallpaperInstance::Type::Video) {
            instance->setVideoSoundVolume(vol);
        }
    }
}

void WallpaperManager::videoPauseOrResume()
{
    for (WallpaperInstance* instance : wallpaperInstances) {
        if (instance && instance->type == WallpaperInstance::Type::Video) {
            instance->pauseResume();
        }
    }
}
Window WallpaperManager::findDesktopWindow(Display *display) {
    Window root = DefaultRootWindow(display);
    Atom desktopAtom = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
    Atom netWmWindowTypeAtom = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);

    Window rootReturn, parentReturn;
    Window *children;
    unsigned int nchildren;

    if (XQueryTree(display, root, &rootReturn, &parentReturn, &children, &nchildren) == 0) {
        qWarning("XQueryTree failed");
        return root;
    }

    for (unsigned int i = 0; i < nchildren; i++) {
        Atom actualType;
        int format;
        unsigned long nitems, bytesAfter;
        unsigned char *prop = nullptr;

        if (XGetWindowProperty(display, children[i], netWmWindowTypeAtom,
                               0, (~0L), False, XA_ATOM,
                               &actualType, &format, &nitems, &bytesAfter, &prop) == Success) {
            if (prop) {
                Atom* atoms = (Atom*)prop;
                for (unsigned long j = 0; j < nitems; j++) {
                    if (atoms[j] == desktopAtom) {
                        Window desktopWin = children[i];
                        XFree(prop);
                        if (children)
                            XFree(children);
                        return desktopWin;
                    }
                }
                XFree(prop);
            }
        }
    }
    if (children)
        XFree(children);

    return root;
}

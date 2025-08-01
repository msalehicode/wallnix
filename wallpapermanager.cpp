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

// void WallpaperManager::startWallpaperbyQML()
// {
//     startWallpaper();
// }

// void WallpaperManager::stopWallpaperbyQML()
// {
//     stopWallpaper();
// }

// void WallpaperManager::changeWallpaperbyQML(const QString &videoPath)
// {
//     QString result= setCurrentWallpaperPath(videoPath);
//     if(result=="ok")
//     {
//         stopWallpaper();
//         startWallpaper();
//         settingsManager.setValue("currentWallpaperPath", videoPath);
//         qInfo() << "wallpaper path changed";
//     }
//     else
//     {
//         qInfo() << "could not change wallaper path";
//     }
//     emit resultChangeWallpaperPath(result);

// }

// void WallpaperManager::getCurrentWallpaper()
// {
//     emit currentWallpaperIs(currentWallpaperPath);
// }

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
    if (!labels.isEmpty()) {
        qWarning() << "Wallpaper already running, stop first!";
        return;
    }

    const auto screens = QApplication::screens();
    for (QScreen* screen : screens) {
        QLabel* label = new QLabel(nullptr);
        label->setWindowFlags(Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
        label->setFocusPolicy(Qt::NoFocus);

        QRect geo = screen->geometry();
        label->resize(geo.size());
        label->move(geo.topLeft());

        QMovie* movie = new QMovie(currentWallpaperPath);
        if (!movie->isValid()) {
            qWarning() << "Invalid movie file:" << currentWallpaperPath;
            delete movie;
            delete label;
            continue;
        }

        movie->setScaledSize(geo.size());
        label->setMovie(movie);


        // label->setWindowFlags(Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
        label->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        label->setAttribute(Qt::WA_X11NetWmWindowTypeDesktop);



        movie->start();
        label->show();
        qInfo() << "window id=" << label->winId();
        // XReparentWindow(display, label->winId(), desktopWindow, geo.x(), geo.y());
        XMapWindow(display, label->winId());
        label->raise();

        labels.append(label);
        movies.append(movie);
    }
    XFlush(display);
}

void WallpaperManager::stopWallpaper() {
    for (auto movie : movies) {
        movie->stop();
        // movie->deleteLater();
        delete movie;
    }
    movies.clear();

    for (auto label : labels) {
        label->hide();
        // label->deleteLater();
        delete label;
    }
    labels.clear();

    if (display) {
        XFlush(display);
    }
}

QString WallpaperManager::setCurrentWallpaperPath(const QString &newCurrentWallpaperPath)
{
    QString res;
    if (newCurrentWallpaperPath.endsWith(".gif", Qt::CaseInsensitive))
    {
        if (QFile::exists(newCurrentWallpaperPath))
        {
            currentWallpaperPath = newCurrentWallpaperPath;
            res = "ok";
        }
        else
        {
            qWarning() << "File does not exist:" << newCurrentWallpaperPath;
            res = "file does not exists.";
        }

    }
    else
    {
        res = "file format does not supported.";
        qWarning() << "File format does not supported:" << newCurrentWallpaperPath;
    }

    return res;
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

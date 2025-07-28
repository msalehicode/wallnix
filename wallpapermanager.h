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
// #include <QThread>
#include <QFile>

class WallpaperManager : public QObject {
    Q_OBJECT
public:

    explicit WallpaperManager(QObject *parent = nullptr) : QObject(parent) {
        display = XOpenDisplay(nullptr);
        if (!display) {
            qFatal("Failed to open X11 display");
        }
        desktopWindow = findDesktopWindow(display);
    }

    Q_INVOKABLE void startWallpaperbyQML(const QString &videoPath)
    {
        qInfo() << "start wallpaper";
        startWallpaper(videoPath);
    }
    Q_INVOKABLE void stopWallpaperbyQML()
    {
        qInfo() << "stop wallpaper";
        stopWallpaper();
    }

    ~WallpaperManager() {
        for (auto label : labels) {
            label->deleteLater();
        }
        if (display) {
            XCloseDisplay(display);
        }
    }

    void startWallpaper(const QString &videoPath) {

        if (!QFile::exists(videoPath)) {
            qWarning() << "File does not exist:" << videoPath;
            return;
        }

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

            QMovie* movie = new QMovie(videoPath);
            if (!movie->isValid()) {
                qWarning() << "Invalid movie file:" << videoPath;
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
            // QThread::msleep(500);  // small delay to ensure cleanup
            qInfo()<<"there we go, check now.";
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

    void stopWallpaper() {
        for (auto movie : movies) {
            movie->stop();
            // movie->deleteLater();
            delete movie;
        }
        movies.clear();

        for (auto label : labels) {
            // XUnmapWindow(display, label->winId());
            // XReparentWindow(display, label->winId(), DefaultRootWindow(display), 0, 0);
            label->hide();
            // label->deleteLater();
            delete label;
        }
        labels.clear();

        if (display) {
            XFlush(display);
        }
    }

private:
    Display* display = nullptr;
    Window desktopWindow;
    QList<QLabel*> labels;
    QList<QMovie*> movies;

    Window findDesktopWindow(Display* display) {
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
};

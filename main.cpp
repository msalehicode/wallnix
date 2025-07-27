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

Window findDesktopWindow(Display* display) {
    Window root = DefaultRootWindow(display);
    Atom desktopAtom = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
    Atom netWmWindowTypeAtom = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);

    Window rootReturn, parentReturn;
    Window *children;
    unsigned int nchildren;

    if (XQueryTree(display, root, &rootReturn, &parentReturn, &children, &nchildren) == 0) {
        qWarning() << "XQueryTree failed";
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

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Display* display = XOpenDisplay(nullptr);
    if (!display) {
        qFatal("Failed to open X11 display");
        return -1;
    }

    Window desktopWindow = findDesktopWindow(display);

    const auto screens = app.screens();
    for (QScreen* screen : screens) {
        QLabel* label = new QLabel(nullptr);
        label->setWindowFlags(Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
        label->setFocusPolicy(Qt::NoFocus);

        QRect geo = screen->geometry();
        label->resize(geo.size());
        label->move(geo.topLeft());

        QMovie* movie = new QMovie("/home/mrx/Downloads/vegeta-ultra-ego.3840x2160-ezgif.com-video-to-gif-converter.gif");
        movie->setScaledSize(geo.size());
        label->setMovie(movie);
        movie->start();

        label->show();

        XReparentWindow(display, label->winId(), desktopWindow, geo.x(), geo.y());
        XMapWindow(display, label->winId());
    }

    XFlush(display);
    return app.exec();
}

#include "main.moc"

#include <QApplication>              // Use QApplication, not QGuiApplication
#include <QQmlApplicationEngine>

#include <QQuickWindow>
#include <QCursor>
#include <QQmlContext>

#include "backend.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    Backend backend(&app);

    engine.rootContext()->setContextProperty("backend", &backend);

    const QUrl url(QStringLiteral("qrc:/wallnix/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);

    engine.load(url);


    if (engine.rootObjects().isEmpty())
        return -1;

    // Get QQuickWindow from root QML object and pass it to backend
    if(backend.setWindow(
            qobject_cast<QQuickWindow*>(engine.rootObjects().first()),
            &app) == false)
                return -1;

    return app.exec();
}

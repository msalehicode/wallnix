#ifndef AUTOSTARTMANAGER_H
#define AUTOSTARTMANAGER_H

#include <QObject>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QDebug>

class AutostartManager : public QObject
{
    Q_OBJECT
public:
    explicit AutostartManager(QObject *parent = nullptr);

    bool isAutostartEnabled() const;

    bool setAutostartEnabled(bool enabled);

    bool getStatus() const;
    void setStatus(bool newStatus);

private:
    QString autostartFilePath() const;
    bool status;
};

#endif // AUTOSTARTMANAGER_H

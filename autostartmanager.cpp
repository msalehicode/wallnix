#include "autostartmanager.h"

AutostartManager::AutostartManager(QObject *parent) : QObject(parent)
{
    status=isAutostartEnabled();
}

bool AutostartManager::isAutostartEnabled() const
{
    QString autostartPath = autostartFilePath();
    return QFile::exists(autostartPath);
}

bool AutostartManager::setAutostartEnabled(bool enabled)
{
    QString autostartPath = autostartFilePath();
    QString systemDesktopFile = "/usr/share/applications/wallnix.desktop";

    if (enabled)
    {
        // Copy .desktop file to autostart folder
        if (!QFile::exists(systemDesktopFile))
        {
            qWarning() << "System desktop file does not exist:" << systemDesktopFile;
            return false;
        }
        // Make sure autostart folder exists
        QDir autostartDir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart");
        if (!autostartDir.exists())
        {
            autostartDir.mkpath(".");
        }
        if (QFile::exists(autostartPath))
            QFile::remove(autostartPath);  // overwrite if exists

        if (!QFile::copy(systemDesktopFile, autostartPath))
        {
            qWarning() << "Failed to copy desktop file to autostart";
            return false;
        }

        return true;
    }
    else
    {
        // Remove autostart file if exists
        if (QFile::exists(autostartPath))
        {
            return QFile::remove(autostartPath);
        }
        return true; // already removed
    }
}

QString AutostartManager::autostartFilePath() const
{
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation); //-> ~/.config
    return configPath + "/autostart/wallnix.desktop";
}

bool AutostartManager::getStatus() const
{
    return status;
}

void AutostartManager::setStatus(bool newStatus)
{
    status = newStatus;
}

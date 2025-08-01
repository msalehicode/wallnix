#include "settingsmanager.h"

SettingsManager::SettingsManager(QObject *parent)
    : QObject{parent} , settings{"wallnix", "wallnix"}
{

}

void SettingsManager::setValue(const QString &key, const QVariant &value)
{
    settings.setValue(key, value);
}

QVariant SettingsManager::getValue(const QString &key, const QVariant &defaultValue)
{
    return settings.value(key, defaultValue);
}

void SettingsManager::remove(const QString &key) {
    settings.remove(key);
}

void SettingsManager::initSettings()
{
    if (!settings.contains("is_startup_enabled"))
        settings.setValue("is_startup_enabled", false);

    if (!settings.contains("is_auto_pause_enabled"))
        settings.setValue("is_auto_pause_enabled", false);

    if (!settings.contains("theme"))
        settings.setValue("theme", "dark");

    if (!settings.contains("last_window_width"))
        settings.setValue("last_window_width", 700);

    if (!settings.contains("last_window_height"))
        settings.setValue("last_window_height", 700);

    if (!settings.contains("currentWallpaperPath"))
        settings.setValue("currentWallpaperPath", "/usr/share/wallnix/resources/default-wallpaper.gif");
}

#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QSettings>

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    explicit SettingsManager(QObject* parent = nullptr);

    void setValue(const QString& key, const QVariant& value);
    QVariant getValue(const QString& key, const QVariant& defaultValue = QVariant());
    void remove(const QString& key);
    void initSettings();

private:
    QSettings settings;
};


#endif // SETTINGSMANAGER_H

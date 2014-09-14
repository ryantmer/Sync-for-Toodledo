#include "PropertiesManager.hpp"
#include <QSettings>
#include <QMutex>

PropertiesManager *PropertiesManager::getInstance() {
    static PropertiesManager *singleton = NULL;
    static QMutex mutex;
    if (singleton == NULL) {
        mutex.lock();
        if (!singleton) {
            singleton = new PropertiesManager();
        }
        mutex.unlock();
    }
    return singleton;
}

PropertiesManager::PropertiesManager(QObject *parent) : QObject (parent) {
}

PropertiesManager::~PropertiesManager() {}

bool PropertiesManager::showTaskTime() {
    QSettings settings("ryantmer", "ToodleDo10");
    QVariant v = settings.value("showTaskTime", false);
    return v.toBool();
}
void PropertiesManager::setShowTaskTime(bool show) {
    QSettings settings("ryantmer", "ToodleDo10");
    QVariant v = settings.value("showTaskTime", false);
    bool b = v.toBool();
    if (b != show) {
        settings.setValue("showTaskTime", show);
        emit showTaskTimeChanged(show);
    }
}

bool PropertiesManager::advancedMode() {
    QSettings settings("ryantmer", "ToodleDo10");
    QVariant v = settings.value("advancedMode", false);
    return v.toBool();
}
void PropertiesManager::setAdvancedMode(bool advanced) {
    QSettings settings("ryantmer", "ToodleDo10");
    QVariant v = settings.value("advancedMode", false);
    bool b = v.toBool();
    if (b != advanced) {
        settings.setValue("advancedMode", advanced);
        emit advancedModeChanged(advanced);
    }
}

uint PropertiesManager::lastUpdateTime() {
    QSettings settings("ryantmer", "ToodleDo10");
    QVariant v = settings.value("lastUpdateTime", 0);
    return v.toUInt(NULL);
}
void PropertiesManager::setLastUpdateTime(uint time) {
    QSettings settings("ryantmer", "ToodleDo10");
    QVariant v = settings.value("lastUpdateTime", 0);
    uint u = v.toUInt(NULL);
    if (u != time) {
        settings.setValue("lastUpdateTime", time);
        emit lastUpdateTimeChanged(time);
    }
}

bool PropertiesManager::loggedIn() {
    QSettings settings("ryantmer", "ToodleDo10");
    QVariant v = settings.value("loggedIn", 0);
    return v.toBool();
}
void PropertiesManager::setLoggedIn(bool loggedIn) {
    QSettings settings("ryantmer", "ToodleDo10");
    QVariant v = settings.value("loggedIn", 0);
    bool b = v.toBool();
    if (b != loggedIn) {
        settings.setValue("loggedIn", loggedIn);
        emit loggedInChanged(loggedIn);
    }
}

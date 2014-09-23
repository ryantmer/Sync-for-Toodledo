#include "PropertiesManager.hpp"
#include <QSettings>
#include <QMutex>
#include <QDateTime>
#include <QDebug>

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

void PropertiesManager::updateAccessToken(QString accessToken, qlonglong expiresIn,
        QString refreshToken, QString tokenScope, QString tokenType) {
    this->accessToken = accessToken;
    this->accessTokenExpiry = QDateTime::currentDateTimeUtc().toTime_t() + expiresIn;
    this->refreshToken = refreshToken;
    this->tokenScope = tokenScope;
    this->tokenType = tokenType;
}

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
        qDebug() << "PropertiesManager::showTaskTime changed to" << show;
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
        qDebug() << "PropertiesManager::advancedMode changed to" << advanced;
        emit advancedModeChanged(advanced);
    }
}

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
    QSettings settings("ryantmer", "ToodleDo10");

    this->accessToken = settings.value("accessToken", "").toString();
    this->accessTokenExpiry = settings.value("accessTokenExpiry", 0).toUInt(NULL);
    this->refreshToken = settings.value("refreshToken", "6d026d6819de22418dc34dfbaf429d9b855d5030").toString();
    this->refreshTokenExpiry = settings.value("refreshTokenExpiry", 0).toUInt(NULL);
    this->tokenScope = settings.value("tokenScope", "").toString();
    this->tokenType = settings.value("tokenType", "").toString();
}
PropertiesManager::~PropertiesManager() {}

void PropertiesManager::updateAccessToken(QString accessToken, qlonglong expiresIn,
        QString refreshToken, QString tokenScope, QString tokenType) {
    this->accessToken = accessToken;
    this->accessTokenExpiry = QDateTime::currentDateTimeUtc().toTime_t() + expiresIn;
    this->refreshToken = refreshToken;
    this->refreshTokenExpiry = QDateTime::currentDateTimeUtc().toTime_t() + 2592000; //30 days from now
    this->tokenScope = tokenScope;
    this->tokenType = tokenType;

    QSettings settings("ryantmer", "ToodleDo10");
    settings.setValue("accessToken", this->accessToken);
    settings.setValue("accessTokenExpiry", this->accessTokenExpiry);
    settings.setValue("refreshToken", this->refreshToken);
    settings.setValue("refreshTokenExpiry", this->refreshTokenExpiry);
    settings.setValue("tokenScope", this->tokenScope);
    settings.setValue("tokenType", this->tokenType);
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

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
    this->refreshToken = settings.value("refreshToken", "").toString();
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

void PropertiesManager::clearTokens() {
    this->accessToken = "";
    this->accessTokenExpiry = 0;
    this->refreshToken = "";
    this->refreshTokenExpiry = 0;
    this->tokenScope = "";
    this->tokenType = "";

    QSettings settings("ryantmer", "ToodleDo10");
    settings.setValue("accessToken", this->accessToken);
    settings.setValue("accessTokenExpiry", this->accessTokenExpiry);
    settings.setValue("refreshToken", this->refreshToken);
    settings.setValue("refreshTokenExpiry", this->refreshTokenExpiry);
    settings.setValue("tokenScope", this->tokenScope);
    settings.setValue("tokenType", this->tokenType);
}

bool PropertiesManager::showCompletedTasks() {
    QSettings settings("ryantmer", "ToodleDo10");
    QVariant v = settings.value("showCompletedTasks", false);
    return v.toBool();
}
void PropertiesManager::setShowCompletedTasks(bool show) {
    QSettings settings("ryantmer", "ToodleDo10");
    QVariant v = settings.value("showCompletedTasks", false);
    bool b = v.toBool();
    if (b != show) {
        settings.setValue("showCompletedTasks", show);
        qDebug() << "PropertiesManager::showCompletedTasks changed to" << show;
        emit showCompletedTasksChanged(show);
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

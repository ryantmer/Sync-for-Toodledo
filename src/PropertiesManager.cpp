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
    QSettings settings("ryantmer", "SyncForToodledo");

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

    QSettings settings("ryantmer", "SyncForToodledo");
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

    QSettings settings("ryantmer", "SyncForToodledo");
    settings.setValue("accessToken", this->accessToken);
    settings.setValue("accessTokenExpiry", this->accessTokenExpiry);
    settings.setValue("refreshToken", this->refreshToken);
    settings.setValue("refreshTokenExpiry", this->refreshTokenExpiry);
    settings.setValue("tokenScope", this->tokenScope);
    settings.setValue("tokenType", this->tokenType);
}

//Delay after which a task is removed from UI after being checked
int PropertiesManager::completedDelay() {
    QSettings settings("ryantmer", "SyncForToodledo");
    return settings.value("completedDelay", 3000).toInt(NULL);
}
void PropertiesManager::setCompletedDelay(int delay) {
    QSettings settings("ryantmer", "SyncForToodledo");
    int d = settings.value("completedDelay", 3000).toInt(NULL);
    if (d != delay) {
        settings.setValue("completedDelay", delay);
        qDebug() << Q_FUNC_INFO << "Changed completedDelay to" << delay;
    }
}

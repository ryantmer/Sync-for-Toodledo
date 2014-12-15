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

    accessToken = settings.value("accessToken", "").toString();
    accessTokenExpiry = settings.value("accessTokenExpiry", 0).toUInt(NULL);
    refreshToken = settings.value("refreshToken", "").toString();
    refreshTokenExpiry = settings.value("refreshTokenExpiry", 0).toUInt(NULL);
    tokenScope = settings.value("tokenScope", "").toString();
    tokenType = settings.value("tokenType", "").toString();

    accountInfo = QVariantMap();
}
PropertiesManager::~PropertiesManager() {}

void PropertiesManager::updateAccessToken(QString newAccessToken, qlonglong expiresIn,
        QString newRefreshToken, QString newTokenScope, QString newTokenType) {
    accessToken = newAccessToken;
    accessTokenExpiry = QDateTime::currentDateTimeUtc().toTime_t() + expiresIn;
    refreshToken = newRefreshToken;
    refreshTokenExpiry = QDateTime::currentDateTimeUtc().toTime_t() + 2592000; //30 days from now
    tokenScope = newTokenScope;
    tokenType = newTokenType;

    QSettings settings("ryantmer", "SyncForToodledo");
    settings.setValue("accessToken", accessToken);
    settings.setValue("accessTokenExpiry", accessTokenExpiry);
    settings.setValue("refreshToken", refreshToken);
    settings.setValue("refreshTokenExpiry", refreshTokenExpiry);
    settings.setValue("tokenScope", tokenScope);
    settings.setValue("tokenType", tokenType);
}

void PropertiesManager::clearTokens() {
    accessToken = "";
    accessTokenExpiry = 0;
    refreshToken = "";
    refreshTokenExpiry = 0;
    tokenScope = "";
    tokenType = "";

    QSettings settings("ryantmer", "SyncForToodledo");
    settings.setValue("accessToken", accessToken);
    settings.setValue("accessTokenExpiry", accessTokenExpiry);
    settings.setValue("refreshToken", refreshToken);
    settings.setValue("refreshTokenExpiry", refreshTokenExpiry);
    settings.setValue("tokenScope", tokenScope);
    settings.setValue("tokenType", tokenType);
}

//Number of days to go back for completed tasks
int PropertiesManager::completedTaskAge() {
    QSettings settings("ryantmer", "SyncForToodledo");
    return settings.value("completedTaskAge", 1).toInt(NULL);
}
void PropertiesManager::setCompletedTaskAge(int days) {
    QSettings settings("ryantmer", "SyncForToodledo");
    int d = settings.value("completedTaskAge", 1).toInt(NULL);
    if (d != days) {
        settings.setValue("completedTaskAge", days);
        qDebug() << Q_FUNC_INFO << "Changed completedDelay to" << days;
    }
}

//Date format, read-only (from account info)
int PropertiesManager::dateFormat() {
    return accountInfo.value("dateformat").toInt(NULL);
}

void PropertiesManager::onLogOut() {
    clearTokens();
    accountInfo = QVariantMap();
}

#include <bb/data/JsonDataAccess>
#include <QMutex>
#include <QDateTime>
#include <bb/data/JsonDataAccess>
#include <bb/PackageInfo>
#include "LoginManager.hpp"
#include "PropertiesManager.hpp"

const QString LoginManager::authorizeUrl = QString("https://api.toodledo.com/3/account/authorize.php");
const QString LoginManager::tokenUrl = QString("https://api.toodledo.com/3/account/token.php");
const QString LoginManager::_credentials = QString("");

using namespace bb::data;

LoginManager *LoginManager::getInstance() {
    static LoginManager *singleton = NULL;
    static QMutex mutex;
    if (singleton == NULL) {
        mutex.lock();
        if (!singleton) {
            singleton = new LoginManager();
        }
        mutex.unlock();
    }
    return singleton;
}

LoginManager::LoginManager(QObject *parent) : QObject(parent) {
    _loggedIn = false;
    _networkAccessManager = new QNetworkAccessManager(this);
    _propMan = PropertiesManager::getInstance();

    //Timer that fires when access token expires (2 hours)
    _accessTokenTimer = new QTimer(this);
    _accessTokenTimer->setSingleShot(true);

    if (QDateTime::currentDateTimeUtc().toTime_t() < _propMan->accessTokenExpiry) {
        _loggedIn = true;
        _accessTokenTimer->start(
                (_propMan->accessTokenExpiry - QDateTime::currentDateTimeUtc().toTime_t()) * 1000);
    }
    //No timer for refresh token, which expires every 30 days.
    //Who the hell keeps an app open for 30 days straight?

    bool isOk;
    isOk = connect(_networkAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onTokenRequestFinished(QNetworkReply*)));
    Q_ASSERT(isOk);
    isOk = connect(this, SIGNAL(refreshTokenExpired()),
            this, SLOT(onRefreshTokenExpired()));
    Q_ASSERT(isOk);
    isOk = connect(this, SIGNAL(accessTokenExpired()),
            this, SLOT(onAccessTokenExpired()));
    Q_ASSERT(isOk);
    isOk = connect(_accessTokenTimer, SIGNAL(timeout()),
            this, SLOT(onAccessTokenExpired()));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);
}

LoginManager::~LoginManager() {}

bool LoginManager::isLoggedIn() {
    if (_propMan->accessTokenExpiry < QDateTime::currentDateTimeUtc().toTime_t()) {
        if (_propMan->refreshTokenExpiry < QDateTime::currentDateTimeUtc().toTime_t()) {
            emit refreshTokenExpired(); //implicitly refreshes access token as well
        } else {
            emit accessTokenExpired();
        }
    }
    return _loggedIn;
}

QUrl LoginManager::getAuthorizeUrl() {
    QUrl url(authorizeUrl);
    url.addQueryItem("response_type", "code");
    url.addQueryItem("client_id", "ToodleDo10");
    url.addQueryItem("state", getState());
    url.addEncodedQueryItem("scope", "basic tasks notes write");

    return url;
}

QString LoginManager::getState() {
    if (_appState.isNull()) {
        _appState = QUuid::createUuid().toString().remove("{").remove("}");
    }
    return _appState;
}

void LoginManager::refreshRefreshToken(QString authCode) {
    //Gets called when user clicks "Authorize" in the login WebView
    QUrl url(tokenUrl);
    QNetworkRequest req(url);

    QUrl data;
    data.addQueryItem("grant_type", "authorization_code");
    data.addQueryItem("code", authCode);
    bb::PackageInfo packageInfo;
    data.addQueryItem("version", packageInfo.version());
    QString auth = QString("Basic " + _credentials.toAscii().toBase64());

    req.setRawHeader(QByteArray("Authorization"), auth.toAscii());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    _networkAccessManager->post(req, data.encodedQuery());
}

void LoginManager::refreshAccessToken() {
    //Gets called automatically whenever the access token expires
    QUrl url(tokenUrl);
    QNetworkRequest req(url);

    QUrl urlData;
    urlData.addQueryItem("grant_type", "refresh_token");
    urlData.addQueryItem("refresh_token", PropertiesManager::getInstance()->refreshToken);
    urlData.addQueryItem("version", QString::number(1));

    QString auth = QString("Basic " + _credentials.toAscii().toBase64());
    req.setRawHeader("Authorization", auth.toAscii());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    qDebug() << Q_FUNC_INFO << url;
    qDebug() << Q_FUNC_INFO << urlData;

    _networkAccessManager->post(req, urlData.encodedQuery());
}

void LoginManager::onLogOut() {
    _loggedIn = false;
    _propMan->clearTokens();
    emit toast("Logged out");
}

void LoginManager::onRefreshTokenExpired() {
    qWarning() << Q_FUNC_INFO << "Refresh token expired (30 days)";
}

void LoginManager::onAccessTokenExpired() {
    //Automatically refresh access token using refresh token
    qWarning() << Q_FUNC_INFO << "Access token expired (2 hours)";
    refreshAccessToken();
}

void LoginManager::onTokenRequestFinished(QNetworkReply *reply) {
    QString response = reply->readAll();

    JsonDataAccess jda;
    QVariantMap dataMap = jda.loadFromBuffer(response).value<QVariantMap>();
    if (jda.hasError()) {
        qWarning() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
        qWarning() << Q_FUNC_INFO << response;
        return;
    }

    if (dataMap.contains("errorDesc")) {
        qWarning() << Q_FUNC_INFO << "Toodledo error" <<
                    dataMap.value("errorCode").toInt(NULL) << ":" <<
                    dataMap.value("errorDesc").toString();
        emit toast("Error " + dataMap.value("errorCode").toString() +
                ": " + dataMap.value("errorDesc").toString());
        if (dataMap.value("errorCode").toInt(NULL) == 102) {
            //Can be caused by a few things, but refreshing the refresh token should always clear it
            emit refreshTokenExpired();
        }
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        _propMan->updateAccessToken(dataMap.value("access_token").toString(),
                dataMap.value("expires_in").toLongLong(NULL),
                dataMap.value("refresh_token").toString());
        _loggedIn = true;
        qDebug() << Q_FUNC_INFO << "New refresh token:" << _propMan->refreshToken;
        emit accessTokenRefreshed();
        emit refreshTokenRefreshed();

        //Restart timeout on access token
        _accessTokenTimer->start(
                (_propMan->accessTokenExpiry - QDateTime::currentDateTimeUtc().toTime_t()) * 1000);
    } else {
        qWarning() << Q_FUNC_INFO << "Reply from" << reply->url() << "contains error" << reply->errorString();
        qWarning() << Q_FUNC_INFO << response;
        emit toast("Error " + dataMap.value("errorCode").toString() +
                ": " + dataMap.value("errorDesc").toString());
    }
    reply->deleteLater();
}

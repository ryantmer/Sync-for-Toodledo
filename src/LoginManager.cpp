#include <bb/data/JsonDataAccess>
#include <QMutex>
#include <QDateTime>
#include <bb/PackageInfo>
#include "LoginManager.hpp"
#include "PropertiesManager.hpp"

const QString LoginManager::_credentials = QString("ToodleDo10:api53ed061e20f0f");

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
    _netMan = NetworkManager::getInstance();
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

    bool ok;
    ok = connect(this, SIGNAL(accessTokenExpired()),
            this, SLOT(onAccessTokenExpired()));
    Q_ASSERT(ok);
    ok = connect(_accessTokenTimer, SIGNAL(timeout()),
            this, SLOT(onAccessTokenExpired()));
    Q_ASSERT(ok);
    ok = connect(_netMan, SIGNAL(accessTokenRefreshed(QString, qlonglong)),
            this, SLOT(onAccessTokenRefreshed(QString, qlonglong)));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
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
    QUrl url(_netMan->authorizeUrl);
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
    QUrl url(_netMan->tokenUrl);
    QNetworkRequest req(url);

    QUrl data;
    data.addQueryItem("grant_type", "authorization_code");
    data.addQueryItem("code", authCode);
    bb::PackageInfo packageInfo;
    data.addQueryItem("version", packageInfo.version());
    QString auth = QString("Basic " + _credentials.toAscii().toBase64());

    req.setRawHeader(QByteArray("Authorization"), auth.toAscii());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    _netMan->sendRequest(req, data.encodedQuery());
}

void LoginManager::refreshAccessToken() {
    //Gets called automatically whenever the access token expires
    QUrl url(_netMan->tokenUrl);
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

    _netMan->sendRequest(req, urlData.encodedQuery());
}

void LoginManager::onLogOut() {
    _loggedIn = false;
    emit toast("Logged out");
}

void LoginManager::onAccessTokenExpired() {
    //Automatically refresh access token using refresh token
    qWarning() << Q_FUNC_INFO << "Access token expired (2 hours)";
    refreshAccessToken();
}

void LoginManager::onAccessTokenRefreshed(QString newToken, qlonglong expiresIn) {
    //Restart access token timer for 2 hours
    _loggedIn = true;
    _accessTokenTimer->start(
            (_propMan->accessTokenExpiry - QDateTime::currentDateTimeUtc().toTime_t()) * 1000);
    Q_UNUSED(newToken);
    Q_UNUSED(expiresIn);
}

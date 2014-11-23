/*
 * LoginManager keeps track of the app's connectivity status to ToodleDo.
 *
 * It will automatically keep track of the access code's expiry, and use the
 * refresh token to renew it as required.
 *
 * Other classes can check whether the app is logged in by calling
 * LoginManager::isLoggedIn().
 */


#include <QMutex>
#include <QDateTime>
#include <bb/data/JsonDataAccess>
#include "LoginManager.hpp"
#include "PropertiesManager.hpp"

const QString LoginManager::authorizeUrl = QString("https://api.toodledo.com/3/account/authorize.php");
const QString LoginManager::tokenUrl = QString("https://api.toodledo.com/3/account/token.php");
const QString LoginManager::credentials = QString("");

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
    loggedIn = false;
    _networkAccessManager = new QNetworkAccessManager(this);
    _propMan = PropertiesManager::getInstance();

    //Timer that fires when access token expires (2 hours)
    accessTokenTimer = new QTimer(this);
    accessTokenTimer->setSingleShot(true);

    if (QDateTime::currentDateTimeUtc().toTime_t() < _propMan->accessTokenExpiry) {
        loggedIn = true;
        accessTokenTimer->start(
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
    isOk = connect(accessTokenTimer, SIGNAL(timeout()),
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
    return this->loggedIn;
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
    if (this->_appState.isNull()) {
        this->_appState = QUuid::createUuid().toString().remove("{").remove("}");
    }
    return this->_appState;
}

void LoginManager::refreshRefreshToken(QString authCode) {
    //Gets called when user clicks "Authorize" in the login WebView
    QUrl url(tokenUrl);
    QNetworkRequest req(url);

    QUrl data;
    data.addQueryItem("grant_type", "authorization_code");
    data.addQueryItem("code", authCode);
    //TODO: update version number
    data.addQueryItem("version", "7");
    QString auth = QString("Basic " + credentials.toAscii().toBase64());

    req.setRawHeader(QByteArray("Authorization"), auth.toAscii());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    _networkAccessManager->post(req, data.encodedQuery());
}

void LoginManager::refreshAccessToken() {
    //Gets called automatically whenever the access token expires
    QUrl url(tokenUrl);
    QNetworkRequest req(url);

    QUrl data;
    data.addQueryItem("grant_type", "refresh_token");
    data.addQueryItem("refresh_token", PropertiesManager::getInstance()->refreshToken);
    data.addQueryItem("version", QString::number(1));

    QString auth = QString("Basic " + credentials.toAscii().toBase64());
    req.setRawHeader("Authorization", auth.toAscii());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    _networkAccessManager->post(req, data.encodedQuery());
}

void LoginManager::onLoggedOut() {
    this->loggedIn = false;
    this->_propMan->clearTokens();
    emit toast("Logged out");
}

void LoginManager::onRefreshTokenExpired() {
    qDebug() << Q_FUNC_INFO << "Refresh token expired (30 days)";
}

void LoginManager::onAccessTokenExpired() {
    //Automatically refresh access token using refresh token
    qDebug() << Q_FUNC_INFO << "Access token expired (2 hours)";
    refreshAccessToken();
}

void LoginManager::onTokenRequestFinished(QNetworkReply *reply) {
    QString response = reply->readAll();

    bb::data::JsonDataAccess jda;
    QVariantMap data = jda.loadFromBuffer(response).value<QVariantMap>();
    if (jda.hasError()) {
        qDebug() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
        qDebug() << Q_FUNC_INFO << response;
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        if (reply->url().toString().contains(tokenUrl)) {
            qDebug() << Q_FUNC_INFO << "New tokens received";
            this->_propMan->updateAccessToken(data.value("access_token").toString(),
                    data.value("expires_in").toLongLong(NULL),
                    data.value("refresh_token").toString(),
                    data.value("scope").toString(),
                    data.value("token_type").toString());
            this->loggedIn = true;
            emit accessTokenRefreshed();
            emit refreshTokenRefreshed();

            qDebug() << Q_FUNC_INFO << "New refresh token:" << this->_propMan->refreshToken;
            //Restart timeout on access token
            accessTokenTimer->start(
                    (_propMan->accessTokenExpiry - QDateTime::currentDateTimeUtc().toTime_t()) * 1000);
        }
    } else {
        //ToodleDo will come back with various error codes if there's a problem
        QVariantMap errorMap = jda.loadFromBuffer(response).value<QVariantMap>();
        if (jda.hasError()) {
            qDebug() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
            qDebug() << Q_FUNC_INFO << response;
            return;
        }

        qDebug() << Q_FUNC_INFO << "ToodleDo error" <<
                errorMap.value("errorCode").toInt(NULL) << ":" <<
                errorMap.value("errorDesc").toString();
    }
    reply->deleteLater();
}

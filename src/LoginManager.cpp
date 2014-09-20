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
const QString LoginManager::credentials = QString("ToodleDo10:api53ed061e20f0f");

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

LoginManager::LoginManager() {
    _networkAccessManager = new QNetworkAccessManager(this);
    QTimer *refreshTokenTimer = new Timer(this);
    QTimer *accessTokenTimer = new Timer(this);

    loggedIn = true;

    bool isOk;
    isOk = connect(_networkAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onTokenRequestFinished(QNetworkReply*)));
    Q_ASSERT(isOk);
    isOk = connect(this, SIGNAL(refreshTokenExpired()),
            this, SLOT(onRefreshTokenExpired()));
    Q_ASSERT(isOk);
    isOk = connect(this, SIGNAL(accessTokenExpired()),
            this, SLOT(onAccessTokenExpired()));
    Q_UNUSED(isOk);

    //Check if refresh token is valid (30-day expiry)
    if (PropertiesManager::getInstance()->refreshTokenExpiry < QDateTime::currentDateTimeUtc().toTime_t()) {
        loggedIn = false;
        emit refreshTokenExpired();
    }
    //Check if access token is valid (2-hour expiry)
    if (PropertiesManager::getInstance()->accessTokenExpiry < QDateTime::currentDateTimeUtc().toTime_t()) {
        loggedIn = false;
        emit accessTokenExpired();
    }
}

LoginManager::~LoginManager() {}

void LoginManager::refreshRefreshToken(QString authCode) {
    //Gets called when user clicks "Authorize" in the login WebView
    QUrl url(tokenUrl);
    QNetworkRequest url(url);

    QUrl data;
    data.addQueryItem("grant_type", "authorization_code");
    data.addQueryItem("code", authCode);
    data.addQueryItem("version", QString::number(1));
    QString auth = QString("Basic " + credentials.toAscii().toBase64());

    req.setRawHeader(QByteArray("Authorization"), auth.toAscii());

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

    _networkAccessManager->post(req, data.encodedQuery());
}

void LoginManager::onRefreshTokenExpired() {
    qDebug() << "Refresh token expired (30 days)";
    refreshRefreshToken();
}

void LoginManager::onAccessTokenExpired() {
    qDebug() << "Access token expired (2 hours)";
    refreshAccessToken();
}

void LoginManager::onTokenRequestFinished(QNetworkReply *reply) {
    //Parse the reply and emit the access code itself
    QString response = reply->readAll();
    qDebug() << "Received LoginManager::onPostFinished reply";

    if (reply->error() == QNetworkReply::NoError) {
        bb::data::JsonDataAccess jda;
        QVariantMap data = jda.loadFromBuffer(response).value<QVariantMap>();

        if (jda.hasError()) {
            qDebug() << "LoginManager::onPostFinished Error:";
            qDebug() << jda.error();
        } else {
            PropertiesManager::getInstance()->updateAccessToken(data.value("access_token").toString(),
                    data.value("expires_in").toLongLong(NULL), data.value("refresh_token").toString(),
                    data.value("scope").toString(), data.value("token_type").toString());
            this->loggedIn = true;
            emit accessTokenRefreshed();
            emit refreshTokenRefreshed();
        }
    }
    reply->deleteLater();
}

//
//QUrl LoginManager::getAuthorizeUrl() {
//    QUrl url(authorizeUrl);
//    url.addQueryItem("response_type", "code");
//    url.addQueryItem("client_id", "ToodleDo10");
//    url.addQueryItem("state", getAppState());
//    url.addEncodedQueryItem("scope", "basic tasks notes folders");
//
//    return url;
//}
//
//void LoginManager::getAccessToken(QString authCode) {
//    QUrl url(tokenUrl);
//    QNetworkRequest req(url);
//
//    //Build up POST data
//    QUrl data;
//    data.addQueryItem("grant_type", "authorization_code");
//    data.addQueryItem("code", authCode);
//    data.addQueryItem("version", QString::number(1));
//    QString auth = QString("Basic " + credentials.toAscii().toBase64());
//
//    req.setRawHeader(QByteArray("Authorization"), auth.toAscii());
//    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
//
//    _networkAccessManager->post(req, data.encodedQuery());
//}
//
//QString LoginManager::getAppState() {
//    if (appState.isEmpty()) {
//        appState = generateAppState();
//    }
//    return appState;
//}
//
//QString LoginManager::generateAppState() {
//    return QUuid::createUuid().toString().remove("{").remove("}");
//}
//
//void LoginManager::onPostFinished(QNetworkReply *reply) {
//    //Parse the reply and emit the access code itself
//    QString response = reply->readAll();
//    qDebug() << "Received LoginManager::onPostFinished reply";
//
//    if (reply->error() == QNetworkReply::NoError) {
//        bb::data::JsonDataAccess jda;
//        QVariantMap data = jda.loadFromBuffer(response).value<QVariantMap>();
//
//        if (jda.hasError()) {
//            qDebug() << "LoginManager::onPostFinished Error:";
//            qDebug() << jda.error();
//        } else {
//            PropertiesManager::getInstance()->updateAccessToken(data.value("access_token").toString(),
//                    data.value("expires_in").toLongLong(NULL), data.value("refresh_token").toString(),
//                    data.value("scope").toString(), data.value("token_type").toString());
//            loggedIn(true);
//            emit accessTokenReceived(PropertiesManager::getInstance()->accessToken);
//        }
//    }
//    reply->deleteLater();
//}

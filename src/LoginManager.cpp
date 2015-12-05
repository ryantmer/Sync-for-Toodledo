#include <bb/data/JsonDataAccess>
#include <QMutex>
#include <QDateTime>
#include <bb/PackageInfo>
#include "LoginManager.hpp"
#include "PropertiesManager.hpp"

const QString LoginManager::_credentials = QString("ToodleDo10:api53ed061e20f0f");
const QString LoginManager::authorizeUrl = QString("https://api.toodledo.com/3/account/authorize.php");
const QString LoginManager::tokenUrl = QString("https://api.toodledo.com/3/account/token.php");

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

LoginManager::LoginManager(QObject *parent)
:   QObject(parent),
    _netConfMan(new QNetworkConfigurationManager(this)),
    _netAccMan(new QNetworkAccessManager(this)),
    _loggedIn(false),
    _connected(false),
    _propMan(PropertiesManager::getInstance()),
    _accessTokenTimer(new QTimer(this)),
    _appState(QString::null)
{
    //Timer that fires when access token expires (2 hours)
    _accessTokenTimer->setSingleShot(true);

    if (QDateTime::currentDateTimeUtc().toTime_t() < _propMan->accessTokenExpiry) {
        _loggedIn = true;
        _accessTokenTimer->start(
                (_propMan->accessTokenExpiry - QDateTime::currentDateTimeUtc().toTime_t()) * 1000);
    }
    //No timer for refresh token, which expires every 30 days.
    //Who the hell keeps an app open for 30 days straight?

    bool ok;
    ok = connect(_accessTokenTimer, SIGNAL(timeout()), this, SLOT(refreshAccessToken()));
    Q_ASSERT(ok);
    ok = connect(_netConfMan, SIGNAL(onlineStateChanged(bool)), this, SLOT(onOnlineStateChanged(bool)));
    Q_ASSERT(ok);
    ok = connect(_netAccMan, SIGNAL(finished(QNetworkReply*)), this, SLOT(onFinished(QNetworkReply*)));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

LoginManager::~LoginManager() {}

bool LoginManager::isLoggedIn() {
    if (!_connected) {
        return false;
    }

    if (_propMan->accessTokenExpiry < QDateTime::currentDateTimeUtc().toTime_t()) {
        if (_propMan->refreshTokenExpiry < QDateTime::currentDateTimeUtc().toTime_t()) {
            // Let the UI know that we need to log in again (implicitly refreshes access token as well)
            qWarning() << Q_FUNC_INFO << "Refresh token expired (30 days)";
            emit refreshTokenExpired();
        } else {
            // Automatically refresh access token using refresh token
            qWarning() << Q_FUNC_INFO << "Access token expired (2 hours)";
            refreshAccessToken();
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

    //Tells UI to show activity indicator
    emit networkRequestStarted();
    qDebug() << Q_FUNC_INFO << "Sending" << data.encodedQuery() << "to" << req.url().toString();
    _netAccMan->post(req, data.encodedQuery());
}

void LoginManager::refreshAccessToken() {
    //Gets called automatically whenever the access token expires
    QUrl url(tokenUrl);
    QNetworkRequest req(url);

    QUrl data;
    data.addQueryItem("grant_type", "refresh_token");
    data.addQueryItem("refresh_token", PropertiesManager::getInstance()->refreshToken);
    data.addQueryItem("version", QString::number(1));

    QString auth = QString("Basic " + _credentials.toAscii().toBase64());
    req.setRawHeader("Authorization", auth.toAscii());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //Tells UI to show activity indicator
    emit networkRequestStarted();
    qDebug() << Q_FUNC_INFO << "Sending" << data.encodedQuery() << "to" << req.url().toString();
    _netAccMan->post(req, data.encodedQuery());
}

/*
 * Slots
 */
void LoginManager::onOnlineStateChanged(bool online)
{
    qDebug() << Q_FUNC_INFO << "Network changed to" << online;
    _connected = online;
    emit networkStateChanged(_connected);
}

void LoginManager::onFinished(QNetworkReply *reply)
{
    QString response = reply->readAll();
    qDebug() << Q_FUNC_INFO << "LoginManager received" << response;

    JsonDataAccess jda;
    QVariantMap replyData = jda.loadFromBuffer(response).value<QVariantMap>();
    if (jda.hasError()) {
        qWarning() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
        return;
    }

    qDebug() << Q_FUNC_INFO << "New refresh token received:" << replyData.value("refresh_token").toString();
    qDebug() << Q_FUNC_INFO << "New access token received:" << replyData.value("access_token").toString();
    _propMan->setAccessToken(replyData.value("access_token").toString(), replyData.value("expires_in").toLongLong(NULL));
    _propMan->setRefreshToken(replyData.value("refresh_token").toString());
    _loggedIn = true;
    _accessTokenTimer->start((_propMan->accessTokenExpiry - QDateTime::currentDateTimeUtc().toTime_t()) * 1000);

    // Tells UI to hide activity indicator
    emit networkRequestFinished();
}

void LoginManager::onLogOut() {
    _loggedIn = false;
    emit toast("Logged out");
}

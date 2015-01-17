#include <QMutex>
#include <QDebug>
#include <bb/data/JsonDataAccess>
#include "NetworkManager.hpp"

const QString NetworkManager::authorizeUrl = QString("https://api.toodledo.com/3/account/authorize.php");
const QString NetworkManager::tokenUrl = QString("https://api.toodledo.com/3/account/token.php");

using namespace bb::data;

NetworkManager *NetworkManager::getInstance() {
    static NetworkManager *singleton = NULL;
    static QMutex mutex;
    if (singleton == NULL) {
        mutex.lock();
        if (!singleton) {
            singleton = new NetworkManager();
        }
        mutex.unlock();
    }
    return singleton;
}

NetworkManager::NetworkManager() {
    _netConfigManager = new QNetworkConfigurationManager(this);
    _netAccessManager = new QNetworkAccessManager(this);
    _connected = _netConfigManager->isOnline();

    bool ok;
    ok = connect(_netConfigManager, SIGNAL(onlineStateChanged(bool)),
            this, SLOT(onOnlineStateChanged(bool)));
    Q_ASSERT(ok);
    ok = connect(_netAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onFinished(QNetworkReply*)));
    Q_UNUSED(ok);
}
NetworkManager::~NetworkManager() {}

bool NetworkManager::isConnected() {
    return _connected;
}

void NetworkManager::onOnlineStateChanged(bool online) {
    qDebug() << Q_FUNC_INFO << "Network state changed. Online:" << online;
    if (online) {
        _connected = true;
    } else {
        _connected = false;
    }
    emit networkStateChanged(_connected);
}

void NetworkManager::sendRequest(QNetworkRequest request, QByteArray encodedQuery) {
    qDebug() << Q_FUNC_INFO << "Sending" << encodedQuery << "to" << request.url().toString();
    _netAccessManager->post(request, encodedQuery);
}

void NetworkManager::onFinished(QNetworkReply *reply) {
    QString response = reply->readAll();

    qDebug() << Q_FUNC_INFO << response;

    JsonDataAccess jda;
    //Some replies are a list of maps, others are just a single map
    QVariantMap dataMap = jda.loadFromBuffer(response).value<QVariantMap>();
    if (jda.hasError()) {
        qWarning() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
        qWarning() << Q_FUNC_INFO << response;
        return;
    }
    QVariantList dataList = jda.loadFromBuffer(response).value<QVariantList>();
    if (jda.hasError()) {
        qWarning() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
        qWarning() << Q_FUNC_INFO << response;
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        if (dataMap.contains("errorDesc")) {
            qWarning() << Q_FUNC_INFO << "Toodledo error" <<
                        dataMap.value("errorCode").toInt(NULL) << ":" <<
                        dataMap.value("errorDesc").toString();
            emit toast("Error " + dataMap.value("errorCode").toString() +
                    ": " + dataMap.value("errorDesc").toString());
            return;
        }

        QString replyUrl = reply->url().toString(QUrl::RemoveQuery);

        if (replyUrl == tokenUrl) {
            qDebug() << Q_FUNC_INFO << "New refresh token received:" << dataMap.value("refresh_token").toString();
            emit refreshTokenRefreshed(dataMap.value("refresh_token").toString());
            qDebug() << Q_FUNC_INFO << "New access token received:" << dataMap.value("access_token").toString();
            emit accessTokenRefreshed(dataMap.value("access_token").toString(),
                    dataMap.value("expires_in").toLongLong(NULL));
        }
    } else {
        qWarning() << Q_FUNC_INFO << "Reply from" << reply->url() << "contains error" << reply->errorString();
        qWarning() << Q_FUNC_INFO << response;
        emit toast(QString("Error %1: %2").arg(dataMap.value("errorCode").toString())
                .arg(dataMap.value("errorDesc").toString()));
    }
    reply->deleteLater();
}

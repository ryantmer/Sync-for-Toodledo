#include <QMutex>
#include "NetworkManager.hpp"

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
    _networkConfigurationManager = new QNetworkConfigurationManager(this);
    _networkSession = new QNetworkSession(
            _networkConfigurationManager->configurationFromIdentifier("bps:ti0"), this);
    _networkAccessManager = new QNetworkAccessManager(this);

    bool isOk;
    isOk = connect(_networkSession, SIGNAL(stateChanged(QNetworkSession::State)), this, SLOT(onNetworkStateChanged(QNetworkSession::State)));
    Q_ASSERT(isOk);
    isOk = connect(_networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onNetworkRequestFinished(QNetworkReply*)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);
}

NetworkManager::~NetworkManager() {}

void NetworkManager::get(QString urlString, QVariantMap urlParams) {
    QUrl url(urlString);
    QVariantMap::iterator i;
    for (i = urlParams.begin(); i != urlParams.end(); ++i) {
        url.addQueryItem(i.key(), i.value().toString());
    }
    _networkAccessManager->get(QNetworkRequest(url));
}

void NetworkManager::onNetworkStateChanged(QNetworkSession::State state) {
    qDebug() << "NETWORK STATE CHANGED: " << state;
    bool connected = false;
    if (state == QNetworkSession::Connected) {
        connected = true;
    }
    emit networkStatusChanged(connected);
}

void NetworkManager::onNetworkRequestFinished(QNetworkReply* reply) {
    QUrl url = reply->url();
    QNetworkReply::NetworkError error = reply->error();
    if (error != QNetworkReply::NoError) {
        qDebug() << "Network response error from " << url << " due to " << error;
        qDebug() << reply->readAll();
        emit networkResponseFailed(url, error);
    } else {
        QString response = QString(reply->readAll());
        emit networkResponse(url, response);
    }
    reply->deleteLater();
}

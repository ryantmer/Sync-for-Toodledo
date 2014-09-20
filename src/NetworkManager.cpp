/*
 * NetworkManager keeps track of the phone's network status.
 *
 * When the status changes, it will internally update its connected status.
 *
 * Other classes can check whether the phone is connected to a network by
 * calling NetworkManager::isConnected().
 */

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
    _netConfigManager = new QNetworkConfigurationManager(this);
    _netSession = new QNetworkSession(_netConfigManager->configurationFromIdentifier("bps:ti0"), this);

    bool isOk;
    isOk = connect(_netSession, SIGNAL(stateChanged(QNetworkSession::State)),
            this, SLOT(ononNetworkStateChanged(QNetworkSession::State)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);
}
NetworkManager::~NetworkManager() {}

bool NetworkManager::isConnected() {
    return this->connected;
}

void NetworkManager::onNetworkStateChanged(QNetworkSession::State state) {
    if (state == QNetworkSession::Connected) {
        this->connected = true;
    } else {
        this->connected = false;
    }

    emit networkStateChanged(this->connected);
}

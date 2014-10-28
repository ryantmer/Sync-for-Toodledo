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
    this->_connected = _netConfigManager->isOnline();

    bool isOk;
    isOk = connect(_netConfigManager, SIGNAL(onlineStateChanged(bool)),
            this, SLOT(onOnlineStateChanged(bool)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);
}
NetworkManager::~NetworkManager() {}

bool NetworkManager::isConnected() {
    return this->_connected;
}

void NetworkManager::onOnlineStateChanged(bool online) {
    if (online) {
        this->_connected = true;
    } else {
        this->_connected = false;
    }
    emit networkStateChanged(this->_connected);
}

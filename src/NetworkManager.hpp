#ifndef NETWORKMANAGER_HPP_
#define NETWORKMANAGER_HPP_

#include <QtNetwork>

class NetworkManager: public QObject {
    Q_OBJECT

public:
    static NetworkManager *getInstance();
    NetworkManager();
    virtual ~NetworkManager();

    bool isConnected();

signals:
    void networkStateChanged(bool activeConnection);

private slots:
    void onNetworkStateChanged(QNetworkSession::State state);

private:
    QNetworkConfigurationManager *_netConfigManager;
    QNetworkSession *_netSession;

    bool connected;
};

#endif /* NETWORKMANAGER_HPP_ */

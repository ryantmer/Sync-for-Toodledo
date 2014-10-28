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
    void toast(QString message);

private slots:
    void onOnlineStateChanged(bool online);

private:
    QNetworkConfigurationManager *_netConfigManager;
    bool _connected;
};

#endif /* NETWORKMANAGER_HPP_ */

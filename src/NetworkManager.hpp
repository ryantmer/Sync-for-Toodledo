#ifndef NETWORKMANAGER_HPP_
#define NETWORKMANAGER_HPP_

#include <QtNetwork>

class NetworkManager : public QObject {
    Q_OBJECT

public:
    static NetworkManager *getInstance();
    NetworkManager();
    virtual ~NetworkManager();

    void get(QString urlString, QVariantMap urlParams);

signals:
    void networkResponse(QUrl url, QString response);
    void networkResponseFailed(QUrl url, int error);
    void networkStatusChanged(bool connectionActive);

public slots:
    void onNetworkStateChanged(QNetworkSession::State state);
    void onNetworkRequestFinished(QNetworkReply* reply);

private:
    QNetworkConfigurationManager *_networkConfigurationManager;
    QNetworkSession *_networkSession;
    QNetworkAccessManager *_networkAccessManager;
};

#endif /* NETWORKMANAGER_HPP_ */

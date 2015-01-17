#ifndef NETWORKMANAGER_HPP_
#define NETWORKMANAGER_HPP_

#include <QtNetwork>

class NetworkManager: public QObject {
    Q_OBJECT

public:
    static NetworkManager *getInstance();
    NetworkManager();
    virtual ~NetworkManager();

    static const QString authorizeUrl;
    static const QString tokenUrl;
    static const QString getUrl;
    static const QString editUrl;
    static const QString addUrl;
    static const QString deleteUrl;

    bool isConnected();
    void sendRequest(QNetworkRequest request, QByteArray encodedQuery);

signals:
    void networkStateChanged(bool activeConnection);
    void toast(QString message);
    void accessTokenRefreshed(QString newToken, qlonglong expiresIn);
    void refreshTokenRefreshed(QString newToken);
    void networkRequestStarted();
    void networkRequestFinished();
    void getReply(QString replyUrl, QVariantList dataList);
    void addReply(QString replyUrl, QVariantList dataList);
    void editReply(QString replyUrl, QVariantList dataList);
    void removeReply(QString replyUrl, QVariantList dataList);

private slots:
    void onOnlineStateChanged(bool online);
    void onFinished(QNetworkReply *reply);

private:
    QNetworkConfigurationManager *_netConfigManager;
    QNetworkAccessManager *_netAccessManager;
    bool _connected;
};

#endif /* NETWORKMANAGER_HPP_ */

#ifndef LOGINMANAGER_HPP_
#define LOGINMANAGER_HPP_

#include <QtNetwork>
#include <QTimer>
#include <QUuid>
#include "PropertiesManager.hpp"

class LoginManager : public QObject {
    Q_OBJECT

public:
    static LoginManager *getInstance();
    LoginManager(QObject *parent = NULL);
    virtual ~LoginManager();

    static const QString authorizeUrl;
    static const QString tokenUrl;

    bool isLoggedIn();
    QUrl getAuthorizeUrl();
    QString getState();
    void refreshRefreshToken(QString authCode);
    void refreshAccessToken();

public slots:
    void onOnlineStateChanged(bool online);
    void onFinished(QNetworkReply *reply);
    void onTimeout();
    void onLogOut();

signals:
    void refreshTokenExpired();
    void accessTokenRefreshed();
    void networkRequestStarted();
    void networkRequestFinished();
    void networkStateChanged(bool online);
    void toast(QString message);

private:
    QNetworkConfigurationManager *_netConfMan;
    QNetworkAccessManager *_netAccMan;
    bool _loggedIn;
    bool _connected;
    PropertiesManager *_propMan;
    QTimer *_accessTokenTimer;
    QString _appState;

    static const QString _credentials;
};

#endif /* LOGINMANAGER_HPP_ */

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
    void onRefreshTokenExpired();
    void onAccessTokenExpired();
    void onTokenRequestFinished(QNetworkReply *reply);
    void onLoggedOut();

signals:
    void accessTokenReceived(QString accessToken);
    void refreshTokenExpired();
    void accessTokenExpired();
    void refreshTokenRefreshed();
    void accessTokenRefreshed();
    void toast(QString message);

private:
    static const QString credentials;
    QNetworkAccessManager *_networkAccessManager;
    PropertiesManager *_propMan;
    bool loggedIn;
    QString _appState;
    QTimer *accessTokenTimer;
};

#endif /* LOGINMANAGER_HPP_ */

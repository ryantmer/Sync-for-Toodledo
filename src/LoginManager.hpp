#ifndef LOGINMANAGER_HPP_
#define LOGINMANAGER_HPP_

#include <QtNetwork>
#include <QTimer>
#include <QUuid>
#include "PropertiesManager.hpp"
#include "NetworkManager.hpp"

class LoginManager : public QObject {
    Q_OBJECT

public:
    static LoginManager *getInstance();
    LoginManager(QObject *parent = NULL);
    virtual ~LoginManager();

    bool isLoggedIn();
    QUrl getAuthorizeUrl();
    QString getState();
    void refreshRefreshToken(QString authCode);
    void refreshAccessToken();

public slots:
    void onAccessTokenExpired();
    void onAccessTokenRefreshed(QString newToken, qlonglong expiresIn);
    void onLogOut();

signals:
    void refreshTokenExpired();
    void accessTokenExpired();
    void toast(QString message);

private:
    bool _loggedIn;
    NetworkManager *_netMan;
    PropertiesManager *_propMan;
    QTimer *_accessTokenTimer;
    QString _appState;
    static const QString _credentials;
};

#endif /* LOGINMANAGER_HPP_ */

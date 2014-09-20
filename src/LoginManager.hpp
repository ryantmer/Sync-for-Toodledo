#ifndef LOGINMANAGER_HPP_
#define LOGINMANAGER_HPP_

#include <QtNetwork>

class LoginManager : public QObject {
    Q_OBJECT

public:
    static LoginManager *getInstance();
    LoginManager();
    virtual ~LoginManager();

    static const QString authorizeUrl;
    static const QString tokenUrl;

    bool loggedIn();
    void loggedIn(bool loggedIn);
    QUrl getAuthorizeUrl();
    void getAccessToken(QString authCode);
    QString getAppState();

public slots:
    void onPostFinished(QNetworkReply *reply);

signals:
    void accessTokenReceived(QString accessToken);

private:
    QString generateAppState();
    QNetworkAccessManager *_networkAccessManager;
    QString appState;
    static const QString credentials;

    bool loggedIn;
};

#endif /* LOGINMANAGER_HPP_ */

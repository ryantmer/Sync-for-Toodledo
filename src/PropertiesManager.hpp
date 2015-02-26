#ifndef PROPERTIESMANAGER_HPP_
#define PROPERTIESMANAGER_HPP_

#include "NetworkManager.hpp"

class PropertiesManager : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(int completedTaskAge READ completedTaskAge WRITE setCompletedTaskAge NOTIFY completedTaskAgeChanged);
    Q_PROPERTY(int dateFormat READ dateFormat NOTIFY dateFormatChanged);

    static PropertiesManager *getInstance();
    PropertiesManager(QObject *parent = 0);
    virtual ~PropertiesManager();
    int completedTaskAge();
    void setCompletedTaskAge(int days);
    int dateFormat();
    void clearTokens();

    QString accessToken;
    uint accessTokenExpiry;
    QString refreshToken;
    uint refreshTokenExpiry;

    //Account info
    QVariantMap accountInfo;

signals:
    void completedTaskAgeChanged(int delay);
    void dateFormatChanged(int format);
    void toast(QString message);

public slots:
    void onLogOut();
    void onAccessTokenRefreshed(QString newToken, qlonglong expiresIn);
    void onRefreshTokenRefreshed(QString newToken);

private:
    NetworkManager *_netMan;
};

#endif /* PROPERTIESMANAGER_HPP_ */

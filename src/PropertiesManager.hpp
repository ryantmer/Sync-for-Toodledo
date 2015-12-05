#ifndef PROPERTIESMANAGER_HPP_
#define PROPERTIESMANAGER_HPP_

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
    void setAccessToken(QString newToken, qlonglong expiresIn);
    void setRefreshToken(QString newToken);

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
};

#endif /* PROPERTIESMANAGER_HPP_ */

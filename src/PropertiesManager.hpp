#ifndef PROPERTIESMANAGER_HPP_
#define PROPERTIESMANAGER_HPP_

class PropertiesManager : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(int completedTaskAge READ completedTaskAge WRITE setCompletedTaskAge NOTIFY completedTaskAgeChanged);
    Q_PROPERTY(int dateFormat READ dateFormat CONSTANT);

    static PropertiesManager *getInstance();
    PropertiesManager(QObject *parent = 0);
    virtual ~PropertiesManager();
    void saveProperties();

    int completedTaskAge();
    void setCompletedTaskAge(int days);
    int dateFormat();

    void updateAccessToken(QString accessToken, qlonglong expiresIn, QString refreshToken);
    void clearTokens();

    QString accessToken;
    uint accessTokenExpiry;
    QString refreshToken;
    uint refreshTokenExpiry;

    //Account info
    QVariantMap accountInfo;

signals:
    void completedTaskAgeChanged(int delay);
    void toast(QString message);

public slots:
    void onLogOut();
};

#endif /* PROPERTIESMANAGER_HPP_ */

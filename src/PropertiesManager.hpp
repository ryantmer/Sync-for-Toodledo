#ifndef PROPERTIESMANAGER_HPP_
#define PROPERTIESMANAGER_HPP_

class PropertiesManager : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(int completedTaskAge READ completedTaskAge WRITE setCompletedTaskAge NOTIFY completedTaskAgeChanged);

    static PropertiesManager *getInstance();
    PropertiesManager(QObject *parent = 0);
    virtual ~PropertiesManager();
    void saveProperties();

    int completedTaskAge();
    void setCompletedTaskAge(int days);

    void updateAccessToken(QString accessToken, qlonglong expiresIn, QString refreshToken, QString tokenScope, QString tokenType);
    void clearTokens();

    QString accessToken;
    uint accessTokenExpiry;
    QString refreshToken;
    uint refreshTokenExpiry;
    QString tokenScope;
    QString tokenType;

    //Account info fields
    QVariantMap accountInfo;

signals:
    void completedTaskAgeChanged(int delay);
    void toast(QString message);

private:
};

#endif /* PROPERTIESMANAGER_HPP_ */

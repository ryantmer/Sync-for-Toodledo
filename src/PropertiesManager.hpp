#ifndef PROPERTIESMANAGER_HPP_
#define PROPERTIESMANAGER_HPP_

class PropertiesManager : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(int completedDelay READ completedDelay WRITE setCompletedDelay NOTIFY completedDelayChanged);

    static PropertiesManager *getInstance();
    PropertiesManager(QObject *parent = 0);
    virtual ~PropertiesManager();
    void saveProperties();

    int completedDelay();
    void setCompletedDelay(int delay);

    void updateAccessToken(QString accessToken, qlonglong expiresIn, QString refreshToken, QString tokenScope, QString tokenType);
    void clearTokens();

    QString accessToken;
    uint accessTokenExpiry;
    QString refreshToken;
    uint refreshTokenExpiry;
    QString tokenScope;
    QString tokenType;

signals:
    void completedDelayChanged(int delay);
    void toast(QString message);

private:
};

#endif /* PROPERTIESMANAGER_HPP_ */

#ifndef PROPERTIESMANAGER_HPP_
#define PROPERTIESMANAGER_HPP_

class PropertiesManager : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(bool showTaskTime READ showTaskTime WRITE setShowTaskTime NOTIFY showTaskTimeChanged);
    Q_PROPERTY(bool advancedMode READ advancedMode WRITE setAdvancedMode NOTIFY advancedModeChanged);

    static PropertiesManager *getInstance();
    PropertiesManager(QObject *parent = NULL);
    virtual ~PropertiesManager();

    bool showTaskTime();
    void setShowTaskTime(bool show);
    bool advancedMode();
    void setAdvancedMode(bool advanced);

    void updateAccessToken(QString accessToken, qlonglong expiresIn, QString refreshToken, QString tokenScope, QString tokenType);

    QString accessToken;
    uint accessTokenExpiry;
    QString refreshToken;
    uint refreshTokenExpiry;
    QString tokenScope;
    QString tokenType;

signals:
    void showTaskTimeChanged(bool show);
    void advancedModeChanged(bool advanced);

private:
};

#endif /* PROPERTIESMANAGER_HPP_ */

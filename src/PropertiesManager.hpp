#ifndef PROPERTIESMANAGER_HPP_
#define PROPERTIESMANAGER_HPP_

class PropertiesManager : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(bool showCompletedTasks READ showCompletedTasks WRITE setShowCompletedTasks NOTIFY showCompletedTasksChanged);
    Q_PROPERTY(bool advancedMode READ advancedMode WRITE setAdvancedMode NOTIFY advancedModeChanged);

    static PropertiesManager *getInstance();
    PropertiesManager(QObject *parent = NULL);
    virtual ~PropertiesManager();

    bool showCompletedTasks();
    void setShowCompletedTasks(bool show);
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
    void advancedModeChanged(bool advanced);
    void showCompletedTasksChanged(bool show);

private:
};

#endif /* PROPERTIESMANAGER_HPP_ */

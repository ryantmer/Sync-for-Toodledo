#ifndef TOODLEDOTEN_HPP_
#define TOODLEDOTEN_HPP_

#include <QObject>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/Container>
#include <bb/cascades/Page>
#include <bb/cascades/WebView>
#include <bb/cascades/ScrollView>

#include "PropertiesManager.hpp"
#include "CustomDataModel.hpp"
#include "NetworkManager.hpp"
#include "LoginManager.hpp"

using namespace bb::cascades;

class ToodledoTen : public QObject {
    Q_OBJECT

public:
    Q_PROPERTY(CustomDataModel *taskDataModel READ taskDataModel CONSTANT);
    Q_PROPERTY(CustomDataModel *folderDataModel READ folderDataModel CONSTANT);
    Q_PROPERTY(CustomDataModel *completedTaskDataModel READ completedTaskDataModel CONSTANT);
    Q_PROPERTY(CustomDataModel *contextDataModel READ contextDataModel CONSTANT);
    Q_PROPERTY(CustomDataModel *goalDataModel READ goalDataModel CONSTANT);
    Q_PROPERTY(CustomDataModel *locationDataModel READ locationDataModel CONSTANT);

    ToodledoTen();
    virtual ~ToodledoTen();
    CustomDataModel *taskDataModel();
    CustomDataModel *folderDataModel();
    CustomDataModel *completedTaskDataModel();
    CustomDataModel *contextDataModel();
    CustomDataModel *goalDataModel();
    CustomDataModel *locationDataModel();

    Q_INVOKABLE QDateTime unixTimeToDateTime(uint unixTime);
    Q_INVOKABLE uint dateTimeToUnixTime(QDateTime dateTime);
    Q_INVOKABLE QDateTime unixTimeToDateTimeNoOffset(uint unixTime);
    Q_INVOKABLE uint dateTimeToUnixTimeNoOffset(QDateTime dateTime);
    Q_INVOKABLE uint getLengthValue(QDateTime dateTime);
    Q_INVOKABLE QString getVersionNumber();
    Q_INVOKABLE QVariantMap getLocation();

    Q_INVOKABLE void logout();
    Q_INVOKABLE void showToast(QString message);

signals:
    void loggedOut();

public slots:
    void onNetworkStateChanged(bool connected);
    void onWebViewUrlChanged(QUrl url);
    void onAccessTokenRefreshed();
    void onRefreshTokenExpired();
    void onToast(QString message);

private:
    CustomDataModel *_taskDataModel;
    CustomDataModel *_folderDataModel;
    CustomDataModel *_completedTaskDataModel;
    CustomDataModel *_contextDataModel;
    CustomDataModel *_goalDataModel;
    CustomDataModel *_locationDataModel;
    NetworkManager *_networkManager;
    PropertiesManager *_propertiesManager;
    LoginManager *_loginManager;

    NavigationPane *_root;
    Container *_coverRoot;
    Page *_loginPage;
    WebView *_loginWebView;
};

#endif /* TOODLEDOTEN_HPP_ */

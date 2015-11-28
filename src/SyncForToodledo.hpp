#ifndef SYNCFORTOODLEDO_HPP_
#define SYNCFORTOODLEDO_HPP_

#include <QObject>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/TabbedPane>
#include <bb/cascades/Container>
#include <bb/cascades/Sheet>
#include <bb/cascades/WebView>
#include <bb/cascades/ScrollView>
#include <bb/cascades/Dialog>
#include <QtLocationSubset/QGeoPositionInfo>
#include <QtLocationSubset/QGeoPositionInfoSource>

#include "PropertiesManager.hpp"
#include "CustomDataModel.hpp"
#include "NetworkManager.hpp"
#include "LoginManager.hpp"

using namespace bb::cascades;
using namespace QtMobilitySubset;

class SyncForToodledo : public QObject {
    Q_OBJECT

public:
    Q_PROPERTY(CustomDataModel *taskDataModel READ taskDataModel CONSTANT);
    Q_PROPERTY(CustomDataModel *folderDataModel READ folderDataModel CONSTANT);
    Q_PROPERTY(CustomDataModel *completedTaskDataModel READ completedTaskDataModel CONSTANT);
    Q_PROPERTY(CustomDataModel *contextDataModel READ contextDataModel CONSTANT);
    Q_PROPERTY(CustomDataModel *goalDataModel READ goalDataModel CONSTANT);
    Q_PROPERTY(CustomDataModel *locationDataModel READ locationDataModel CONSTANT);

    SyncForToodledo();
    virtual ~SyncForToodledo();

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
    Q_INVOKABLE void getLocation();

    Q_INVOKABLE void logout();
    Q_INVOKABLE void showToast(QString message);

signals:
    void loggedOut();

public slots:
    void onPositionUpdated(const QGeoPositionInfo &pos);
    void onNetworkStateChanged(bool connected);
    void onNetworkRequestStarted(QString message="Loading...");
    void onNetworkRequestFinished();
    void onWebViewUrlChanged(QUrl url);
    void onRefreshTokenExpired();
    void onAccessTokenRefreshed(QString newToken, qlonglong expiresIn);
    void onAccountInfoUpdated();
    void onToast(QString message);

private:
    PropertiesManager *_propertiesManager;
    NetworkManager *_networkManager;
    LoginManager *_loginManager;

    CustomDataModel *_taskDataModel;
    CustomDataModel *_folderDataModel;
    CustomDataModel *_completedTaskDataModel;
    CustomDataModel *_contextDataModel;
    CustomDataModel *_locationDataModel;
    CustomDataModel *_goalDataModel;
    CustomDataModel *_accountInfo;

    TabbedPane *_root;
    Container *_coverRoot;
    Sheet *_loginSheet;
    WebView *_loginWebView;
    Dialog *_activityDialog;
};

#endif /* SYNCFORTOODLEDO_HPP_ */

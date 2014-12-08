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
#include "SenderReceiver.hpp"
#include "LoginManager.hpp"

using namespace bb::cascades;

class ToodledoTen : public QObject {
    Q_OBJECT

public:
    Q_PROPERTY(CustomDataModel *taskDataModel READ taskDataModel CONSTANT);
    Q_PROPERTY(CustomDataModel *folderDataModel READ folderDataModel CONSTANT);

    ToodledoTen();
    virtual ~ToodledoTen();
    CustomDataModel *taskDataModel();
    CustomDataModel *folderDataModel();

    Q_INVOKABLE QDateTime unixTimeToDateTime(uint unixTime);
    Q_INVOKABLE uint dateTimeToUnixTime(QDateTime dateTime);
    Q_INVOKABLE QDateTime unixTimeToDateTimeNoOffset(uint unixTime);
    Q_INVOKABLE uint dateTimeToUnixTimeNoOffset(QDateTime dateTime);
    Q_INVOKABLE uint getLengthValue(QDateTime dateTime);
    Q_INVOKABLE QString getVersionNumber();
    Q_INVOKABLE void refreshTasks();
    Q_INVOKABLE void refreshFolders();
    Q_INVOKABLE void addTask(QVariantMap data);
    Q_INVOKABLE void editTask(QVariantMap oldData, QVariantMap newData);
    Q_INVOKABLE void removeTask(QVariantMap data);
    Q_INVOKABLE void addFolder(QVariantMap data);
    Q_INVOKABLE void editFolder(QVariantMap oldData, QVariantMap newData);
    Q_INVOKABLE void removeFolder(QVariantMap data);
    Q_INVOKABLE QVariantList getFolderList();
    Q_INVOKABLE void logout();
    Q_INVOKABLE void showToast(QString message);

signals:
    void taskAdded(QVariantMap data);
    void taskEdited(QVariantMap oldData, QVariantMap newData);
    void taskRemoved(QVariantMap data);
    void folderAdded(QVariantMap data);
    void folderEdited(QVariantMap oldData, QVariantMap newData);
    void folderRemoved(QVariantMap data);
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
    SenderReceiver *_taskSenderReceiver;
    SenderReceiver *_folderSenderReceiver;
    SenderReceiver *_completedTaskSenderReceiver;
    SenderReceiver *_contextSenderReceiver;
    SenderReceiver *_goalSenderReceiver;
    SenderReceiver *_locationSenderReceiver;
    NetworkManager *_networkManager;
    PropertiesManager *_propertiesManager;
    LoginManager *_loginManager;

    NavigationPane *_root;
    Container *_coverRoot;
    Page *_loginPage;
    WebView *_loginWebView;
};

#endif /* TOODLEDOTEN_HPP_ */

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
#include "TaskDataModel.hpp"
#include "FolderDataModel.hpp"
#include "NetworkManager.hpp"
#include "TaskSenderReceiver.hpp"
#include "FolderSenderReceiver.hpp"
#include "LoginManager.hpp"

using namespace bb::cascades;

class ToodledoTen : public QObject {
    Q_OBJECT

public:
    Q_PROPERTY(TaskDataModel *taskDataModel READ taskDataModel CONSTANT);
    Q_PROPERTY(FolderDataModel *folderDataModel READ folderDataModel CONSTANT);

    ToodledoTen();
    virtual ~ToodledoTen();
    TaskDataModel *taskDataModel();
    FolderDataModel *folderDataModel();

    Q_INVOKABLE QDateTime unixTimeToDateTime(uint unixTime);
    Q_INVOKABLE uint dateTimeToUnixTime(QDateTime dateTime);
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
    void onAppMinimized();
    void onNetworkStateChanged(bool connected);
    void onWebViewUrlChanged(QUrl url);
    void onAccessTokenRefreshed();
    void onRefreshTokenExpired();
    void onToast(QString message);

private:
    TaskDataModel *_taskDataModel;
    FolderDataModel *_folderDataModel;
    TaskSenderReceiver *_taskSenderReceiver;
    FolderSenderReceiver *_folderSenderReceiver;
    NetworkManager *_networkManager;
    PropertiesManager *_propertiesManager;
    LoginManager *_loginManager;

    NavigationPane *_root;
    Container *_coverRoot;
    Page *_loginPage;
    ScrollView *_loginScrollView;
    WebView *_loginWebView;
};

#endif /* TOODLEDOTEN_HPP_ */

#ifndef TOODLEDOTEN_CPP_
#define TOODLEDOTEN_CPP_

#include "ToodleDoTen.hpp"

#include <bb/cascades/Container>
#include <bb/cascades/SceneCover>
#include <bb/system/SystemToast>
#include <bb/system/SystemUiPosition>

using namespace bb::cascades;
using namespace bb::system;

ToodledoTen::ToodledoTen() : QObject() {
    //TODO: Figure out what's not properly parented here, and isn't quitting properly
    qmlRegisterType<TaskDataModel>("TaskUtilities", 1, 0, "TaskDataModel");
    qmlRegisterType<FolderDataModel>("FolderUtilities", 1, 0, "FolderDataModel");

    _propertiesManager = PropertiesManager::getInstance();
    _loginManager = LoginManager::getInstance();
    _networkManager = NetworkManager::getInstance();
    _taskSenderReceiver = new TaskSenderReceiver(this);
    _folderSenderReceiver = new FolderSenderReceiver(this);
    _taskDataModel = new TaskDataModel(this);
    _folderDataModel = new FolderDataModel(this);

    //Create root QML document from main.qml and expose certain variables to QML
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("app", this);
    qml->setContextProperty("propertyManager", _propertiesManager);
    qml->setContextProperty("taskDataModel", _taskDataModel);
    qml->setContextProperty("folderDataModel", _folderDataModel);
    root = qml->createRootObject<NavigationPane>();
    Application::instance()->setScene(root);

    //Expose app to the main listview
    QDeclarativeEngine *engine = QmlDocument::defaultDeclarativeEngine();
    QDeclarativeContext *rootContext = engine->rootContext();
    rootContext->setContextProperty("app", this);

    //Login page, shown if required
    QmlDocument *loginQml = QmlDocument::create("asset:///Login.qml").parent(this);
    loginPage = loginQml->createRootObject<Page>();
    loginWebView = loginPage->findChild<WebView*>("loginWebView");

    bool isOk;

    //Toodledo10 listeners
    isOk = connect(Application::instance(), SIGNAL(thumbnail()),
            this, SLOT(onAppMinimize()));
    Q_ASSERT(isOk);
    isOk = connect(loginWebView, SIGNAL(urlChanged(QUrl)),
            this, SLOT(onWebViewUrlChanged(QUrl)));
    Q_ASSERT(isOk);
    isOk = connect(_loginManager, SIGNAL(accessTokenRefreshed()),
            this, SLOT(onAccessTokenRefreshed()));
    Q_ASSERT(isOk);
    isOk = connect(_loginManager, SIGNAL(refreshTokenExpired()),
            this, SLOT(onRefreshTokenExpired()));
    Q_ASSERT(isOk);
    //YEAH TOAST
    isOk = connect(_propertiesManager, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(isOk);
    isOk = connect(_loginManager, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(isOk);
    isOk = connect(_networkManager, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(isOk);
    isOk = connect(_taskSenderReceiver, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(isOk);
    isOk = connect(_folderSenderReceiver, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(isOk);
    isOk = connect(_taskDataModel, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(isOk);
    isOk = connect(_folderDataModel, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(isOk);

    //LoginManager listeners
    isOk = connect(this, SIGNAL(loggedOut()),
            _loginManager, SLOT(onLoggedOut()));
    Q_ASSERT(isOk);

    //TDM listeners
    isOk = connect(this, SIGNAL(loggedOut()),
            _taskDataModel, SLOT(onLoggedOut()));
    Q_ASSERT(isOk);
    isOk = connect(_taskSenderReceiver, SIGNAL(taskGetReply(QVariantMap)),
            _taskDataModel, SLOT(onTaskEdited(QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(_taskSenderReceiver, SIGNAL(taskEditReply(QVariantMap)),
            _taskDataModel, SLOT(onTaskEdited(QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(_taskSenderReceiver, SIGNAL(taskAddReply(QVariantMap)),
            _taskDataModel, SLOT(onTaskAdded(QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(_taskSenderReceiver, SIGNAL(taskRemoveReply(QVariantMap)),
            _taskDataModel, SLOT(onTaskRemoved(QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(Application::instance(), SIGNAL(aboutToQuit()),
            _taskDataModel, SLOT(onAboutToQuit()));
    Q_ASSERT(isOk);

    //FDM listeners
    isOk = connect(this, SIGNAL(loggedOut()),
            _folderDataModel, SLOT(onLoggedOut()));
    Q_ASSERT(isOk);
    isOk = connect(_folderSenderReceiver, SIGNAL(folderGetReply(QVariantMap)),
            _folderDataModel, SLOT(onFolderEdited(QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(_folderSenderReceiver, SIGNAL(folderEditReply(QVariantMap)),
            _folderDataModel, SLOT(onFolderEdited(QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(_folderSenderReceiver, SIGNAL(folderAddReply(QVariantMap)),
            _folderDataModel, SLOT(onFolderAdded(QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(_folderSenderReceiver, SIGNAL(folderRemoveReply(QVariantMap)),
            _folderDataModel, SLOT(onFolderRemoved(QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(Application::instance(), SIGNAL(aboutToQuit()),
            _folderDataModel, SLOT(onAboutToQuit()));
    Q_ASSERT(isOk);

    //TSR listeners
    isOk = connect(this, SIGNAL(taskAdded(QVariantMap)),
            _taskSenderReceiver, SLOT(onTaskAdded(QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(this, SIGNAL(taskEdited(QVariantMap, QVariantMap)),
            _taskSenderReceiver, SLOT(onTaskEdited(QVariantMap, QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(this, SIGNAL(taskRemoved(QVariantMap)),
            _taskSenderReceiver, SLOT(onTaskRemoved(QVariantMap)));
    Q_ASSERT(isOk);

    //FSR listeners
    isOk = connect(this, SIGNAL(folderAdded(QVariantMap)),
            _folderSenderReceiver, SLOT(onFolderAdded(QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(this, SIGNAL(folderEdited(QVariantMap, QVariantMap)),
            _folderSenderReceiver, SLOT(onFolderEdited(QVariantMap, QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(this, SIGNAL(folderRemoved(QVariantMap)),
            _folderSenderReceiver, SLOT(onFolderRemoved(QVariantMap)));
    Q_ASSERT(isOk);

    Q_UNUSED(isOk);

    refreshTasks();
    refreshFolders();
}
ToodledoTen::~ToodledoTen() {};

TaskDataModel *ToodledoTen::taskDataModel() {
    return _taskDataModel;
}

FolderDataModel *ToodledoTen::folderDataModel() {
    return _folderDataModel;
}

void ToodledoTen::showToast(QString message) {
    SystemToast *toast = new SystemToast(this);
    toast->setBody(message);
    toast->setPosition(SystemUiPosition::MiddleCenter);
    toast->show();
}

/*
 * Q_INVOKABLE functions begin
 */
QDateTime ToodledoTen::unixTimeToDateTime(uint unixTime) {
    return QDateTime::fromTime_t(unixTime);
}

uint ToodledoTen::dateTimeToUnixTime(QDateTime dateTime) {
    return dateTime.toTime_t();
}

void ToodledoTen::refreshTasks() {
    if (_networkManager->isConnected()) {
        if (_loginManager->isLoggedIn()) {
            _taskSenderReceiver->fetchAllTasks();
        } else {
            qDebug() << Q_FUNC_INFO << "LoginManager indicated not logged in";
        }
    } else {
        qDebug() << Q_FUNC_INFO << "NetworkManager indicated no network connection";
        showToast("No network connection!");
    }
}

void ToodledoTen::refreshFolders() {
    if (_networkManager->isConnected()) {
        if (_loginManager->isLoggedIn()) {
            _folderSenderReceiver->fetchAllFolders();
        } else {
            qDebug() << Q_FUNC_INFO << "LoginManager indicated not logged in";
        }
    } else {
        qDebug() << Q_FUNC_INFO << "NetworkManager indicated no network connection";
    }
}

void ToodledoTen::addTask(QVariantMap data) {
    emit taskAdded(data);
}

void ToodledoTen::editTask(QVariantMap oldData, QVariantMap newData) {
    if (oldData != newData) {
        emit taskEdited(oldData, newData);
    }
}

void ToodledoTen::removeTask(QVariantMap data) {
    emit taskRemoved(data);
}

void ToodledoTen::addFolder(QVariantMap data) {
    emit folderAdded(data);
}

void ToodledoTen::editFolder(QVariantMap oldData, QVariantMap newData) {
    if (oldData != newData) {
        emit folderEdited(oldData, newData);
    }
}

void ToodledoTen::removeFolder(QVariantMap data) {
    emit folderRemoved(data);
}

QVariantList ToodledoTen::getFolderList() {
    return _folderDataModel->getFolderList();
}

void ToodledoTen::logout() {
    emit loggedOut();
}
/*
 * Q_INVOKABLE functions end
 */

/*
 * Slots begin
 */
void ToodledoTen::onWebViewUrlChanged(QUrl url) {
    if (url.hasQueryItem("code") && url.hasQueryItem("state")) {
        if (url.queryItemValue("state") == _loginManager->getState()) {
            //Get authCode from webview's URL
            QString authCode = url.queryItemValue("code");
            //Start access token request
            _loginManager->refreshRefreshToken(authCode);
            //Remove webview from nav pane
            root->pop();
        } else {
            qDebug() << Q_FUNC_INFO << "State didn't match";
        }
    }
}

void ToodledoTen::onAccessTokenRefreshed() {
    //access token is automatically refreshed when it expires using refresh token
    //When a new access token is received, refresh
    refreshTasks();
    refreshFolders();
}

void ToodledoTen::onRefreshTokenExpired() {
    //emitted by LoginManager when refresh token is no longer valid (30-day expiry)
    //when this occurs, user has to log in again
    showToast("Please log in!");
    root->push(loginPage);
    loginWebView->setUrl(_loginManager->getAuthorizeUrl().toString());
}

void ToodledoTen::onAppMinimize() {
    //Add cover QML for app
    QmlDocument *qmlCover = QmlDocument::create("asset:///Cover.qml").parent(this);
    qmlCover->setContextProperty("app", this);
    Container *rootContainer = qmlCover->createRootObject<Container>();
    //TODO: Fix this memory leak - cover is created each time app is minimized
    SceneCover *cover = SceneCover::create().content(rootContainer);
    Application::instance()->setCover(cover);
}

void ToodledoTen::onToast(QString message) {
    showToast(message);
}
/*
 * Slots end
 */

#endif /* TOODLEDOTEN_CPP_ */

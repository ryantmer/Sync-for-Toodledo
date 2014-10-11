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

    _propertiesManager = PropertiesManager::getInstance();
    _loginManager = LoginManager::getInstance();
    _networkManager = NetworkManager::getInstance();
    _taskSenderReceiver = new TaskSenderReceiver(this);
    _dataModel = new TaskDataModel(this);

    //Create root QML document from main.qml and expose certain variables to QML
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("app", this);
    qml->setContextProperty("propertyManager", this->_propertiesManager);
    qml->setContextProperty("dataModel", this->_dataModel);
    root = qml->createRootObject<NavigationPane>();
    Application::instance()->setScene(root);

    //Expose app to the main listview
    QDeclarativeEngine *engine = QmlDocument::defaultDeclarativeEngine();
    QDeclarativeContext *rootContext = engine->rootContext();
    rootContext->setContextProperty("app", this);

    //Login page, shown if required
    QmlDocument *loginQml = QmlDocument::create("asset:///Login.qml").parent(this);
    this->loginPage = loginQml->createRootObject<Page>();
    this->loginWebView = loginPage->findChild<WebView*>("loginWebView");

    bool isOk;
    //Toodledo10 listens for minimize signal
    isOk = connect(Application::instance(), SIGNAL(thumbnail()),
            this, SLOT(onAppMinimize()));
    Q_ASSERT(isOk);
    //Toodledo10 listens for WebView changing URL
    isOk = connect(loginWebView, SIGNAL(urlChanged(QUrl)),
            this, SLOT(onWebViewUrlChanged(QUrl)));
    Q_ASSERT(isOk);
    //ToodleDo10 listens to LoginManager when access token refreshes
    isOk = connect(_loginManager, SIGNAL(accessTokenRefreshed()),
            this, SLOT(onAccessTokenRefreshed()));
    Q_ASSERT(isOk);
    //ToodleDo10 listens to LoginManager when refresh token expires
    isOk = connect(_loginManager, SIGNAL(refreshTokenExpired()),
            this, SLOT(onRefreshTokenExpired()));
    Q_ASSERT(isOk);
    //LoginManager listens for loggedOut signal from UI
    isOk = connect(this, SIGNAL(loggedOut()),
            _loginManager, SLOT(onLoggedOut()));
    Q_ASSERT(isOk);
    //Datamodel listens for loggedOut signal from UI
    isOk = connect(this, SIGNAL(loggedOut()),
            _dataModel, SLOT(onLoggedOut()));
    Q_ASSERT(isOk);
    //Datamodel listens for task get reply signal from TSR
    isOk = connect(_taskSenderReceiver, SIGNAL(taskGetReply(QVariantMap)),
            _dataModel, SLOT(onTaskEdited(QVariantMap)));
    Q_ASSERT(isOk);
    //Datamodel listens for task update (edit) reply signal from TSR
    isOk = connect(_taskSenderReceiver, SIGNAL(taskEditReply(QVariantMap)),
            _dataModel, SLOT(onTaskEdited(QVariantMap)));
    Q_ASSERT(isOk);
    //Datamodel listens for task add reply signal from TSR
    isOk = connect(_taskSenderReceiver, SIGNAL(taskAddReply(QVariantMap)),
            _dataModel, SLOT(onTaskAdded(QVariantMap)));
    Q_ASSERT(isOk);
    //Datamodel listens for task remove reply signal from TSR
    isOk = connect(_taskSenderReceiver, SIGNAL(taskRemoveReply(QVariantMap)),
            _dataModel, SLOT(onTaskRemoved(QVariantMap)));
    Q_ASSERT(isOk);
    //TSR listens for added signal
    isOk = connect(this, SIGNAL(taskAdded(QVariantMap)),
            _taskSenderReceiver, SLOT(onTaskAdded(QVariantMap)));
    Q_ASSERT(isOk);
    //TSR listens for edited signal
    isOk = connect(this, SIGNAL(taskEdited(QVariantMap, QVariantMap)),
            _taskSenderReceiver, SLOT(onTaskEdited(QVariantMap, QVariantMap)));
    Q_ASSERT(isOk);
    //TSR listens for removed signal
    isOk = connect(this, SIGNAL(taskRemoved(QVariantMap)),
            _taskSenderReceiver, SLOT(onTaskRemoved(QVariantMap)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);

    refresh();
}
ToodledoTen::~ToodledoTen() {};

TaskDataModel *ToodledoTen::dataModel() {
    return this->_dataModel;
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

void ToodledoTen::refresh() {
    if (this->_networkManager->isConnected()) {
        if (this->_loginManager->isLoggedIn()) {
            this->_taskSenderReceiver->fetchAllTasks();
        } else {
            qDebug() << Q_FUNC_INFO << "LoginManager indicated not logged in";
        }
    } else {
        qDebug() << Q_FUNC_INFO << "NetworkManager indicated no network connection";
        showToast("No network connection!");
    }
}

void ToodledoTen::addTask(QVariantMap data) {
    emit taskAdded(data);
}

void ToodledoTen::editTask(QVariantMap oldData, QVariantMap newData) {
    emit taskEdited(oldData, newData);
}

void ToodledoTen::removeTask(QVariantMap data) {
    emit taskRemoved(data);
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
    refresh();
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
/*
 * Slots end
 */

#endif /* TOODLEDOTEN_CPP_ */

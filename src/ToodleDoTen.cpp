#ifndef TOODLEDOTEN_CPP_
#define TOODLEDOTEN_CPP_

#include "ToodleDoTen.hpp"

#include <bb/system/SystemToast>
#include <bb/system/SystemUiPosition>
#include <bb/data/JsonDataAccess>

using namespace bb::cascades;
using namespace bb::system;
using namespace bb::data;

ToodleDoTen::ToodleDoTen() : QObject() {
    qmlRegisterType<TaskDataModel>("TaskUtilities", 1, 0, "TaskDataModel");

    _propertiesManager = PropertiesManager::getInstance();
    _loginManager = LoginManager::getInstance();
    _networkManager = NetworkManager::getInstance();
    _taskSenderReceiver = new TaskSenderReceiver(this);
    _dataModel = new TaskDataModel(this);

    //Create root QML document from main.qml and expose certain variables to QML
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("app", this);
    qml->setContextProperty("propertyManager", _propertiesManager);
    qml->setContextProperty("dataModel", this->_dataModel);
    root = qml->createRootObject<NavigationPane>();
    Application::instance()->setScene(root);

    //Expose app to the main listview
    QDeclarativeEngine *engine = QmlDocument::defaultDeclarativeEngine();
    QDeclarativeContext *rootContext = engine->rootContext();
    rootContext->setContextProperty("app", this);

    //Login page, shown if required
    QmlDocument *loginQml = QmlDocument::create("asset:///Login.qml").parent(qml);
    loginPage = loginQml->createRootObject<Page>();
    loginWebView = loginPage->findChild<WebView*>("loginWebView");

    bool isOk;
    //ToodleDo10 listens for WebView changing URL
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
    //ToodleDo10 listens for updated items in datamodel
    isOk = connect(_taskSenderReceiver, SIGNAL(tasksUpdated(QVariantList)),
            this, SLOT(onTasksUpdated(QVariantList)));
    Q_ASSERT(isOk);
    //DataModel listens for added signal from sender-receiver
    isOk = connect(_taskSenderReceiver, SIGNAL(tasksAdded(QVariantList)),
            _dataModel, SLOT(onTasksAdded(QVariantList)));
    Q_ASSERT(isOk);
    //DataModel listens for updated signal from sender-receiver
    isOk = connect(_taskSenderReceiver, SIGNAL(tasksUpdated(QVariantList)),
            _dataModel, SLOT(onTasksUpdated(QVariantList)));
    Q_ASSERT(isOk);
    //DataModel listens for removed signal from sender-receiver
    isOk = connect(_taskSenderReceiver, SIGNAL(tasksRemoved(QVariantList)),
            _dataModel, SLOT(onTasksRemoved(QVariantList)));
    Q_ASSERT(isOk);
    //Sender-receiver listens for added items in datamodel
    isOk = connect(_dataModel, SIGNAL(taskAdded(QVariantList)),
            _taskSenderReceiver, SLOT(onTaskAdded(QVariantList)));
    Q_ASSERT(isOk);
    //Sender-receiver listens for edited items in datamodel
    isOk = connect(_dataModel, SIGNAL(taskEdited(QVariantList)),
            _taskSenderReceiver, SLOT(onTaskEdited(QVariantList)));
    Q_ASSERT(isOk);
    //Sender-receiver listens for removed items in datamodel
    isOk = connect(_dataModel, SIGNAL(taskRemoved(QVariantList)),
            _taskSenderReceiver, SLOT(onTaskRemoved(QVariantList)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);

    refresh();
}
ToodleDoTen::~ToodleDoTen() {};

TaskDataModel *ToodleDoTen::dataModel() {
    return this->_dataModel;
}

void ToodleDoTen::showToast(QString message) {
    SystemToast *toast = new SystemToast(this);
    toast->setBody(message);
    toast->setPosition(SystemUiPosition::MiddleCenter);
    toast->show();
}

/*
 * Q_INVOKABLE functions begin
 */
QDateTime ToodleDoTen::unixTimeToDateTime(uint unixTime) {
    return QDateTime::fromTime_t(unixTime);
}

uint ToodleDoTen::dateTimeToUnixTime(QDateTime dateTime) {
    return dateTime.toTime_t();
}

void ToodleDoTen::refresh() {
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

void ToodleDoTen::addTask(QVariantMap data) {
    this->_dataModel->addTask(data);
}

void ToodleDoTen::editTask(QVariantMap data) {
    this->_dataModel->editTask(data);
}

void ToodleDoTen::removeTask(QVariantMap data) {
    this->_dataModel->removeTask(data);
}

void ToodleDoTen::logout() {
    this->_loginManager->logout();
}
/*
 * Q_INVOKABLE functions end
 */

/*
 * Slots begin
 */
void ToodleDoTen::onWebViewUrlChanged(QUrl url) {
    if (url.hasQueryItem("code") && url.hasQueryItem("state")) {
        if (url.queryItemValue("state") == _loginManager->getState()) {
            //Get authCode from webview's URL
            QString authCode = url.queryItemValue("code");
            //Start access token request
            _loginManager->refreshRefreshToken(authCode);
            //Remove webview from nav pane
            root->pop();
        } else {
            qDebug() << "State didn't match";
        }
    }
}

void ToodleDoTen::onAccessTokenRefreshed() {
    //access token is automatically refreshed when it expires using refresh token
    //When a new access token is received, refresh
    refresh();
}

void ToodleDoTen::onRefreshTokenExpired() {
    //emitted by LoginManager when refresh token is no longer valid (30-day expiry)
    //when this occurs, user has to log in again
    showToast("Please log in!");
    root->push(loginPage);
    loginWebView->setUrl(_loginManager->getAuthorizeUrl().toString());
}

void ToodleDoTen::onTasksUpdated(QVariantList tasks) {
    showToast("Tasks Refreshed!");
    Q_UNUSED(tasks);
}
/*
 * Slots end
 */

#endif /* TOODLEDOTEN_CPP_ */

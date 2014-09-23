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
    _taskRetriever = new TaskRetriever(this);
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
    isOk = connect(loginWebView, SIGNAL(urlChanged(QUrl)),
            this, SLOT(onWebViewUrlChanged(QUrl)));
    Q_ASSERT(isOk);
    isOk = connect(_loginManager, SIGNAL(accessTokenRefreshed()),
            this, SLOT(onAccessTokenRefreshed()));
    Q_ASSERT(isOk);
    isOk = connect(_loginManager, SIGNAL(refreshTokenExpired()),
            this, SLOT(onRefreshTokenExpired()));
    Q_ASSERT(isOk);
    isOk = connect(_taskRetriever, SIGNAL(tasksUpdated(QVariantList)),
            _dataModel, SLOT(onTasksUpdated(QVariantList)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);

    refresh();
}
ToodleDoTen::~ToodleDoTen() {};

TaskDataModel *ToodleDoTen::dataModel() {
    return this->_dataModel;
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
            this->_taskRetriever->fetchAllTasks();
        } else {
            qDebug() << Q_FUNC_INFO << "LoginManager indicated not logged in";
            showToast("Not logged in!");
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

void ToodleDoTen::logout() {
    this->_loginManager->logout();
}
/*
 * Q_INVOKABLE functions end
 */

void ToodleDoTen::showToast(QString message) {
    SystemToast *toast = new SystemToast(this);
    toast->setBody(message);
    toast->setPosition(SystemUiPosition::MiddleCenter);
    toast->show();
}

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
    //When a new access token is received, refresh
    refresh();
}

void ToodleDoTen::onRefreshTokenExpired() {
    //emitted by LoginManager when refresh token is no longer valid (30-day expiry)
    root->push(loginPage);
    loginWebView->setUrl(_loginManager->getAuthorizeUrl().toString());
}
/*
 * Slots end
 */

#endif /* TOODLEDOTEN_CPP_ */

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
    isOk = connect(_loginManager, SIGNAL(accessTokenReceived(QString)),
            this, SLOT(onAccessTokenReceived(QString)));
    Q_ASSERT(isOk);
    isOk = connect(_taskRetriever, SIGNAL(tasksUpdated(QVariantList)),
            _dataModel, SLOT(onTasksUpdated(QVariantList)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);

    //If user isn't logged in, show webview with login page
    if (_loginManager->loggedIn()) {
        refresh();
    } else {
        loginWebView->setUrl(_loginManager->getAuthorizeUrl());
        root->push(loginPage);
    }
}
ToodleDoTen::~ToodleDoTen() {};

QDateTime ToodleDoTen::unixTimeToDateTime(uint unixTime) {
    return QDateTime::fromTime_t(unixTime);
}
uint ToodleDoTen::dateTimeToUnixTime(QDateTime dateTime) {
    return dateTime.toTime_t();
}

TaskDataModel *ToodleDoTen::dataModel() {
    return this->_dataModel;
}

void ToodleDoTen::refresh() {
    if (this->_loginManager->loggedIn()) {
        showToast("Refreshing tasks...");
        _taskRetriever->fetchAllTasks();
    } else {

    }
}

void ToodleDoTen::addTask(QVariantMap data) {
    this->_dataModel->onTaskAdded(data);
}

void ToodleDoTen::editTask(QVariantMap data) {
    this->_dataModel->onTaskEdited(data);
}

void ToodleDoTen::showToast(QString message) {
    SystemToast *toast = new SystemToast(this);
    toast->setBody(message);
    toast->setPosition(SystemUiPosition::MiddleCenter);
    toast->show();
}

void ToodleDoTen::onWebViewUrlChanged(QUrl url) {
    if (url.hasQueryItem("code") && url.hasQueryItem("state")) {
        if (url.queryItemValue("state") == _loginManager->getAppState()) {
            //Get authCode from webview's URL
            QString authCode = url.queryItemValue("code");
            //Start access token request
            _loginManager->getAccessToken(authCode);
            //Remove webview from nav pane
            root->pop();
        } else {
            qDebug() << "State didn't match";
        }
    }
}

//void ToodleDoTen::onAccessTokenExpired() {
//    //When LoginManager tells us the access token expired, user needs to login again
//
//}

void ToodleDoTen::onAccessTokenReceived(QString) {
    //When a new access token is received, refresh
    refresh();
}

#endif /* TOODLEDOTEN_CPP_ */

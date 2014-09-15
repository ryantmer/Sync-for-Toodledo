#ifndef TOODLEDOTEN_CPP_
#define TOODLEDOTEN_CPP_

#include "ToodleDoTen.hpp"
#include "TaskDataModel.hpp"
#include "TaskRetriever.hpp"
#include "PropertiesManager.hpp"
#include "NetworkManager.hpp"

#include <bb/system/SystemToast>
#include <bb/system/SystemUiPosition>
#include <bb/cascades/WebView>

using namespace bb::cascades;
using namespace bb::system;

ToodleDoTen::ToodleDoTen() : QObject() {
    qmlRegisterType<TaskDataModel>("TaskUtilities", 1, 0, "TaskDataModel");

    PropertiesManager *propertyManager = PropertiesManager::getInstance();
    NetworkManager *networkManager = NetworkManager::getInstance();
    this->_taskRetriever = new TaskRetriever(this);
    this->_dataModel = new TaskDataModel(this);

    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("app", this);
    qml->setContextProperty("propertyManager", propertyManager);
    qml->setContextProperty("dataModel", this->_dataModel);
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);

    QDeclarativeEngine *engine = QmlDocument::defaultDeclarativeEngine();
    QDeclarativeContext *rootContext = engine->rootContext();
    rootContext->setContextProperty("app", this);

    bool isOk;
    //NetworkManager does its own connections
    //PropertiesManager has no connections
    //TaskDataModel connections
    isOk = connect(_taskRetriever, SIGNAL(tasksUpdated(QVariantList)),
            this->_dataModel, SLOT(onTasksUpdated(QVariantList)));
    Q_ASSERT(isOk);
    //TaskRetriever connections
    isOk = connect(networkManager, SIGNAL(networkResponse(QUrl, QString)),
            this->_taskRetriever, SLOT(onNetworkResponse(QUrl, QString)));
    Q_ASSERT(isOk);
    isOk = connect(networkManager, SIGNAL(networkResponseFailed(QUrl, int)),
            this->_taskRetriever, SLOT(onNetworkResponseFailed(QUrl, int)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);

    if (!propertyManager->loggedIn()) {
        showToast("Not logged in!");

        WebView *loginPageWebview = root->findChild<WebView*>("loginPageWebview");
        QUrl url("https://api.toodledo.com/3/account/authorize.php");
        url.addQueryItem("response_type", "code");
        url.addQueryItem("client_id", "ToodleDo10");
        url.addQueryItem("state", "random_string_lolololol");
        url.addEncodedQueryItem("scope", "basic tasks notes");
        loginPageWebview->setUrl(url);
    }

    refresh();
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
    //This should actually upload changes, then fetch new changes.
    //For now, it just fetches all tasks that haven't been completed.
    _taskRetriever->fetchAllTasks();
}

void ToodleDoTen::addTask(QVariantMap data) {
    this->_dataModel->onTaskAdded(data);
}

void ToodleDoTen::editTask(QVariantMap data) {
    this->_dataModel->onTaskEdited(data);
}

void ToodleDoTen::clearLocalTasks() {
    this->_dataModel->onLocalTasksRemoved();
}

void ToodleDoTen::showToast(QString message) {
    SystemToast *toast = new SystemToast(this);
    toast->setBody(message);
    toast->setPosition(SystemUiPosition::MiddleCenter);
    toast->show();
}

#endif /* TOODLEDOTEN_CPP_ */

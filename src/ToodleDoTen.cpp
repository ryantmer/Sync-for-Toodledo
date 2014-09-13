#ifndef TOODLEDOTEN_CPP_
#define TOODLEDOTEN_CPP_

#include <bb/cascades/SceneCover>
#include <bb/cascades/Container>

#include "ToodleDoTen.hpp"
#include "TaskDataModel.hpp"
#include "TaskRetriever.hpp"
#include "PropertiesManager.hpp"
#include "NetworkManager.hpp"

using namespace bb::cascades;

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
    //PropertyManager has no connections
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

#endif /* TOODLEDOTEN_CPP_ */

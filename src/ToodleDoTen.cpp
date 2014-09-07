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

    qDebug() << "--> Creating QML...";
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("app", this);
    qml->setContextProperty("propertyManager", propertyManager);
    qml->setContextProperty("dataModel", this->_dataModel);
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);
    qDebug() << "--> Finished.";

    qDebug() << "--> Connecting signals...";
    bool isOk;
    isOk = connect(networkManager, SIGNAL(networkStatusChanged(bool)),
            this, SLOT(onNetworkStatusChanged(bool)));
    Q_ASSERT(isOk);
    isOk = connect(_taskRetriever, SIGNAL(taskUpdated(QVariantMap)),
            this, SLOT(onTaskUpdated(QVariantMap)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);
    qDebug() << "--> Finished.";
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
    //Placeholder. This should actually upload changes, then fetch new changes.
    _taskRetriever->fetchAllTasks();
}

void ToodleDoTen::addTask(QVariantList data) {
    this->_dataModel->onTaskAdded(data);
}

void ToodleDoTen::editTask(QVariantList data) {
    this->_dataModel->onTaskEdited(data);
}

void ToodleDoTen::clearLocalTasks() {
    this->_dataModel->onLocalTasksRemoved();
}

void ToodleDoTen::onTaskUpdated(QVariantMap taskData) {
    qDebug() << "--> Task updated!";
    qDebug() << taskData;
}

void ToodleDoTen::onNetworkStatusChanged(bool connected) {
    PropertiesManager::getInstance()->setNetworkIssues(!connected);
    if (connected) {
        this->refresh();
    }
}

void ToodleDoTen::onRefreshTriggered() {
    _taskRetriever->fetchAllTasks();
}

#endif /* TOODLEDOTEN_CPP_ */

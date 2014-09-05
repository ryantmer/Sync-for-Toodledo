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

    qDebug() << "Creating QML...";
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("app", this);
    qml->setContextProperty("propertyManager", propertyManager);
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);

    _refreshButton = root->findChild<ActionItem*>("refreshButton");
    _addTaskButton = root->findChild<ActionItem*>("addTaskButton");
    _editTaskButton = root->findChild<ActionItem*>("editTaskButton");

    qDebug() << "Connecting signals...";
    bool isOk;
    isOk = connect(networkManager, SIGNAL(networkStatusChanged(bool)),
            this, SLOT(onNetworkStatusChanged(bool)));
    Q_ASSERT(isOk);
    isOk = connect(_refreshButton, SIGNAL(triggered()),
            this, SLOT(onRefreshTriggered()));
    Q_ASSERT(isOk);
    isOk = connect(_taskRetriever, SIGNAL(taskUpdated(QVariantMap)),
            this, SLOT(onTaskUpdated(QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(_addTaskButton, SIGNAL(triggered(QVariant)),
            this->_dataModel, SLOT(onTaskAdded(QVariant)));
    Q_ASSERT(isOk);
    isOk = connect(_editTaskButton, SIGNAL(triggered(QVariant)),
            this->_dataModel, SLOT(onTaskEdited(QVariant)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);
    qDebug() << "Finished.";
}
ToodleDoTen::~ToodleDoTen() {};

TaskDataModel *ToodleDoTen::dataModel() {
    return this->_dataModel;
}

void ToodleDoTen::updateDataModel() {
    _taskRetriever->fetchAllTasks();
}

void ToodleDoTen::onTaskUpdated(QVariantMap taskData) {
    qDebug() << "Task updated!";
    qDebug() << taskData;
}

void ToodleDoTen::onNetworkStatusChanged(bool connected) {
    PropertiesManager::getInstance()->setNetworkIssues(!connected);
    if (connected) {
        this->updateDataModel();
    }
}

void ToodleDoTen::onRefreshTriggered() {
    _taskRetriever->fetchAllTasks();
}

#endif /* TOODLEDOTEN_CPP_ */

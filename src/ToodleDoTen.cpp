#ifndef TOODLEDOTEN_CPP_
#define TOODLEDOTEN_CPP_

#include <bb/cascades/SceneCover>
#include <bb/cascades/Container>

#include "ToodleDoTen.hpp"
#include "TaskDataModel.hpp"
#include "PropertiesManager.hpp"
#include "NetworkManager.hpp"

using namespace bb::cascades;

ToodleDoTen::ToodleDoTen() : QObject() {
    PropertiesManager *propertyManager = PropertiesManager::getInstance();
    NetworkManager *networkManager = NetworkManager::getInstance();

    qDebug() << "Connecting to signals...";
    bool isOk = connect(networkManager, SIGNAL(networkStatusChanged(bool)), this, SLOT(onNetworkStatusChanged(bool)));
    Q_ASSERT(isOk);

    qDebug() << "Creating QML...";
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("app", this);
    qml->setContextProperty("propertyManager", propertyManager);
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);

    qDebug() << "Connecting to fetchTasks...";
    _refreshButton = root->findChild<ActionItem*>("refreshButton");
    isOk = connect(_refreshButton, SIGNAL(triggered()), this, SLOT(onFetchTasks()));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);
}
ToodleDoTen::~ToodleDoTen() {};

TaskDataModel *ToodleDoTen::dataModel() {
    return this->_dataModel;
}

void ToodleDoTen::updateDataModel() {

}

void ToodleDoTen::onTaskUpdated() {
    this->updateDataModel();
}

void ToodleDoTen::onNetworkStatusChanged(bool connected) {
    PropertiesManager::getInstance()->setNetworkIssues(!connected);
    if (connected) {
        this->updateDataModel();
    }
}

void ToodleDoTen::onFetchTasks() {
    QVariantMap urlParameters;
    urlParameters["access_token"] = QString("4675589614eff6243dd86c837a6e45db8c6ba666");
    urlParameters["after"] = QString::number(0);
    urlParameters["fields"] = QString("folder,duedate");
    NetworkManager::getInstance()->get("http://api.toodledo.com/3/tasks/get.php", urlParameters);
}

#endif /* TOODLEDOTEN_CPP_ */

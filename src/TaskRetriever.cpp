#include "TaskRetriever.hpp"
#include "NetworkManager.hpp"
#include "PropertiesManager.hpp"
#include <bb/data/JsonDataAccess>
#include <QDateTime>
#include <ctime>

using namespace bb::data;

TaskRetriever::TaskRetriever(QObject *parent) : QObject(parent) {
    bool isOk;
    isOk = connect(NetworkManager::getInstance(), SIGNAL(networkResponse(QUrl, QString)),
            this, SLOT(onNetworkResponse(QUrl, QString)));
    Q_ASSERT(isOk);
    isOk = connect(NetworkManager::getInstance(), SIGNAL(networkResponseFailed(QUrl, int)),
            this, SLOT(onNetworkResponseFailed(QUrl, int)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);
}

void TaskRetriever::fetchAllTasks() {
    QVariantMap urlParameters;
    urlParameters["access_token"] = QString("e378eb86d3437d2d256508b88e5f411da925d148");
    //Get the lastUpdateTime, then set it to be now for future updates
    urlParameters["after"] = QString::number(PropertiesManager::getInstance()->lastUpdateTime());
    PropertiesManager::getInstance()->setLastUpdateTime(QDateTime::currentMSecsSinceEpoch() / 1000);
    //TODO: THIS DOESN'T WORK.
    qDebug() << PropertiesManager::getInstance()->lastUpdateTime();
    //id, title, modified, completed do not need to be specified, they come anyway
    urlParameters["fields"] = QString("duedate");

    //Send network request
    NetworkManager::getInstance()->get("http://api.toodledo.com/3/tasks/get.php", urlParameters);
}

void TaskRetriever::fetchTask(int taskId) {
    QVariantMap urlParameters;
    urlParameters["access_token"] = QString("e378eb86d3437d2d256508b88e5f411da925d148");
    urlParameters["id"] = QString::number(taskId);
    //id, title, modified, completed do not need to be specified, they come anyway
    urlParameters["fields"] = QString("duedate");

    //Send network request
    NetworkManager::getInstance()->get("http://api.toodledo.com/3/tasks/get.php", urlParameters);
}

void TaskRetriever::onNetworkResponse(QUrl url, QString response) {
    JsonDataAccess jda;
    QVariantList data = jda.loadFromBuffer(response).value<QVariantList>();
    if (jda.hasError()) {
        qDebug() << "Error:" << jda.error();
    }
    emit tasksUpdated(data);
}

void TaskRetriever::onNetworkResponseFailed(QUrl url, int error) {
    int taskId = url.queryItemValue("id").toInt(NULL);
    qDebug() << "Fetching task " << taskId << " failed with error " << error;
    emit taskUpdateFailed(taskId);
}

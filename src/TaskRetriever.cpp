#include "TaskRetriever.hpp"
#include "NetworkManager.hpp"
#include <bb/data/JsonDataAccess>

using namespace bb::data;

//Tasks are received with a UNIX timestamp for all fields, need to convert them
static QVariant unixTimeToDateString(QVariant unixTime) {
    return unixTime;
}
static QVariant dateStringToUnixTime(QVariant dateString) {
    return dateString;
}

TaskRetriever::TaskRetriever(QObject *parent) : QObject(parent) {
    bool isOk = connect(NetworkManager::getInstance(),
            SIGNAL(networkResponse(QUrl, QString)),
            this,
            SLOT(onNetworkResponse(QUrl, QString)));
    Q_ASSERT(isOk);
    isOk = connect(NetworkManager::getInstance(),
            SIGNAL(networkResponseFailed(QUrl, int)),
            this,
            SLOT(onNetworkResponseFailed(QUrl, int)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);
}

void TaskRetriever::fetchAllTasks() {
    QVariantMap urlParameters;
    urlParameters["access_token"] = QString("7253598f96296ebc709cae6f81a4dd849e0b29ce");
    urlParameters["after"] = QString::number(0);
    //id, title, modified and completed do not need to be specified, they come anyway
    urlParameters["fields"] = QString("folder,duedate");
    NetworkManager::getInstance()->get("http://api.toodledo.com/3/tasks/get.php", urlParameters);
}

void TaskRetriever::fetchTask(int taskId) {
    QVariantMap urlParameters;
    urlParameters["access_token"] = QString("7253598f96296ebc709cae6f81a4dd849e0b29ce");
    urlParameters["id"] = QString::number(taskId);
    //id, title, modified and completed do not need to be specified, they come anyway
    urlParameters["fields"] = QString("folder,duedate");
    NetworkManager::getInstance()->get("http://api.toodledo.com/3/tasks/get.php", urlParameters);
}

void TaskRetriever::onNetworkResponse(QUrl url, QString response) {
    int taskId = url.queryItemValue("id").toInt(NULL);
    this->parseTaskUpdateResponse(taskId, response);
}

void TaskRetriever::onNetworkResponseFailed(QUrl url, int error) {
    int taskId = url.queryItemValue("id").toInt(NULL);
    emit taskUpdateFailed(taskId);
}

void TaskRetriever::parseTaskUpdateResponse(int taskId, QString response) {
    qDebug() << taskId << " came back with " << response;
}

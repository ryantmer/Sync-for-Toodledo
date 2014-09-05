#include "TaskRetriever.hpp"
#include "NetworkManager.hpp"
#include "PropertiesManager.hpp"
#include <bb/data/JsonDataAccess>
#include <QDateTime>
#include <ctime>

using namespace bb::data;

//Tasks are received with a UNIX timestamp for all fields, need to convert them
static QDateTime unixTimeToDateTime(uint unixTime) {
    return QDateTime::fromTime_t(unixTime);
}
static uint dateTimeToUnixTime(QDateTime dateTime) {
    return dateTime.toTime_t();
}

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
    urlParameters["access_token"] = QString("a7b40a478c7825b35cf7ca6501702479127cd82e");
    //Get the lastUpdateTime, then set it to be now for future updates
    urlParameters["after"] = QString::number(PropertiesManager::getInstance()->lastUpdateTime());
    PropertiesManager::getInstance()->setLastUpdateTime(QDateTime::currentDateTimeUtc().toTime_t());
    //id, title, modified, completed do not need to be specified, they come anyway
    urlParameters["fields"] = QString("duedate");

    //Send network request
    NetworkManager::getInstance()->get("http://api.toodledo.com/3/tasks/get.php", urlParameters);
}

void TaskRetriever::fetchTask(int taskId) {
    QVariantMap urlParameters;
    urlParameters["access_token"] = QString("a7b40a478c7825b35cf7ca6501702479127cd82e");
    urlParameters["id"] = QString::number(taskId);
    //id, title, modified, completed do not need to be specified, they come anyway
    urlParameters["fields"] = QString("duedate");

    //Send network request
    NetworkManager::getInstance()->get("http://api.toodledo.com/3/tasks/get.php", urlParameters);
}

void TaskRetriever::onNetworkResponse(QUrl url, QString response) {
    qDebug() << url << "came back with" << response;
    QVariantMap data;
    //TODO: Convert response into QVariantMap
    emit taskUpdated(data);
}

void TaskRetriever::onNetworkResponseFailed(QUrl url, int error) {
    int taskId = url.queryItemValue("id").toInt(NULL);
    qDebug() << "Fetching task " << taskId << " failed with error " << error;
    emit taskUpdateFailed(taskId);
}

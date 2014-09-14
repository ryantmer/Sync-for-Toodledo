#include "TaskRetriever.hpp"
#include "NetworkManager.hpp"
#include <bb/data/JsonDataAccess>

using namespace bb::data;

TaskRetriever::TaskRetriever(QObject *parent) : QObject(parent) {
}

void TaskRetriever::fetchAllTasks() {
    QVariantMap urlParameters;

    urlParameters["access_token"] = QString("fa1036d7c3b18d18adc49dadb62740b127a635df");
    //Only get tasks that haven't been completed
    urlParameters["comp"] = QString::number(0);
    //id, title, modified, completed do not need to be specified, they come anyway
    urlParameters["fields"] = QString("duedate");

    //Send network request
    NetworkManager::getInstance()->get("http://api.toodledo.com/3/tasks/get.php", urlParameters);
}

void TaskRetriever::fetchTask(int taskId) {
    QVariantMap urlParameters;

    urlParameters["access_token"] = QString("fa1036d7c3b18d18adc49dadb62740b127a635df");
    urlParameters["id"] = QString::number(taskId);
    //id, title, modified, completed do not need to be specified, they come anyway
    urlParameters["fields"] = QString("duedate");

    //Send network request
    NetworkManager::getInstance()->get("http://api.toodledo.com/3/tasks/get.php", urlParameters);
}

void TaskRetriever::onNetworkResponse(QUrl url, QString response) {
    JsonDataAccess jda;
    QVariantList data = jda.loadFromBuffer(response).value<QVariantList>();
    qDebug() << "Received" << data.first().toMap().value("num").value<qlonglong>() << "tasks from" << url;
//    foreach (QVariant v, data) {
//        qDebug() << v;
//    }
    data.pop_front();
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

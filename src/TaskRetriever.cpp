#include <bb/data/JsonDataAccess>

#include "TaskRetriever.hpp"
#include "PropertiesManager.hpp"
#include "LoginManager.hpp"

const QString TaskRetriever::getUrl = QString("http://api.toodledo.com/3/tasks/get.php");

TaskRetriever::TaskRetriever(QObject *parent) : QObject(parent) {
    _networkAccessManager = new QNetworkAccessManager(this);

    bool isOk;
    isOk = connect(_networkAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onTasksReceived(QNetworkReply*)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);
}

void TaskRetriever::fetchAllTasks() {
    QUrl url(getUrl);
    url.addQueryItem("access_token", PropertiesManager::getInstance()->accessToken);
    url.addQueryItem("comp", QString::number(0));
    url.addEncodedQueryItem("fields", "duedate"); //id, title, modified, completed come automatically

    QNetworkRequest req(url);
    _networkAccessManager->get(req);
}

void TaskRetriever::fetchTask(int taskId) {
    QUrl url(getUrl);
    url.addQueryItem("access_token", PropertiesManager::getInstance()->accessToken);
    url.addQueryItem("id", QString::number(taskId));
    url.addEncodedQueryItem("fields", "duedate"); //id, title, modified, completed come automatically

    QNetworkRequest req(url);
    _networkAccessManager->get(req);
}

void TaskRetriever::onTasksReceived(QNetworkReply *reply) {
    QString response = reply->readAll();
    qDebug() << "Received TaskRetriever::onPostFinished reply";

    if (reply->error() == QNetworkReply::NoError) {
        bb::data::JsonDataAccess jda;
        QVariantList data = jda.loadFromBuffer(response).value<QVariantList>();

        if (jda.hasError()) {
            qDebug() << "TaskRetriever::onTasksReceived Error:";
            qDebug() << jda.error();
        } else {
            //Remove the summary item (gives number of tasks)
            data.pop_front();
            emit tasksUpdated(data);
        }
    }
    reply->deleteLater();
}

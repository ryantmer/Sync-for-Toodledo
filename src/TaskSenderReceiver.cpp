#include <bb/data/JsonDataAccess>

#include "TaskSenderReceiver.hpp"
#include "LoginManager.hpp"

const QString TaskSenderReceiver::getUrl = QString("http://api.toodledo.com/3/tasks/get.php");
const QString TaskSenderReceiver::editUrl = QString("http://api.toodledo.com/3/tasks/edit.php");
const QString TaskSenderReceiver::addUrl = QString("http://api.toodledo.com/3/tasks/add.php");
const QString TaskSenderReceiver::removeUrl = QString("http://api.toodledo.com/3/tasks/delete.php");

TaskSenderReceiver::TaskSenderReceiver(QObject *parent) : QObject(parent) {
    _networkAccessManager = new QNetworkAccessManager(this);
    _propMan = PropertiesManager::getInstance();

    bool isOk;
    isOk = connect(_networkAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onTasksReceived(QNetworkReply*)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);
}

void TaskSenderReceiver::fetchAllTasks() {
    QUrl url(getUrl);
    url.addQueryItem("access_token", PropertiesManager::getInstance()->accessToken);
    url.addQueryItem("comp", QString::number(0)); //only get non-complete tasks
    url.addEncodedQueryItem("fields", "duedate,note"); //id, title, modified, completed come automatically

    QNetworkRequest req(url);
    _networkAccessManager->get(req);
}

void TaskSenderReceiver::fetchTask(int taskId) {
    QUrl url(getUrl);
    url.addQueryItem("access_token", PropertiesManager::getInstance()->accessToken);
    url.addQueryItem("id", QString::number(taskId));
    url.addEncodedQueryItem("fields", "duedate"); //id, title, modified, completed come automatically

    QNetworkRequest req(url);
    _networkAccessManager->get(req);
}

void TaskSenderReceiver::addTasks(QVariantList task) {
    //Task(s) added on site, need to fetch for TaskDataModel
    //TODO: this.
}

void TaskSenderReceiver::updateTasks(QVariantList task) {
    //Task(s) updated on site, need to fetch changes for TaskDataModel
    //TODO: this.
}

void TaskSenderReceiver::onTaskAdded(QVariantList task) {
    //Task was added by user in UI, need to upload new task
    QVariantMap taskMap = task.first().toMap();

    QUrl url(addUrl);
    QNetworkRequest req(url);

    //Build task data string from user's input
    QString encodedData = QString("[{");
    if (taskMap["title"].toString() != "") {
        encodedData.append("\"title\":\"" + taskMap["title"].toString() + "\",");
    }
    if (taskMap["duedate"].toLongLong(NULL) > 0) {
        encodedData.append("\"duedate\":" + taskMap["duedate"].toString() + ",");
    }
    if (taskMap["note"].toString() != "") {
        encodedData.append("\"note\":\"" + taskMap["note"].toString() + "\",");
    }
    encodedData.append("\"ref\":\"taskAdd\"}]");
    //Required for ToodleDo's API to replace some stuff
    encodedData = encodedData.replace("\n", "\\n").replace(" ", "+");
    encodedData = QUrl::toPercentEncoding(encodedData, "\"{}[]+\\,:", "");

    QUrl data;
    data.addQueryItem("access_token", _propMan->accessToken);
    data.addEncodedQueryItem("tasks", encodedData.toAscii());
    data.addQueryItem("fields", "duedate,note");

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    _networkAccessManager->post(req, data.encodedQuery());
}

void TaskSenderReceiver::onTaskEdited(QVariantList task) {
    //Task was edited by user in UI, need to upload changes
    qDebug() << Q_FUNC_INFO << "Task edited, sending to server";
    qDebug() << task;

    QVariantMap taskMap = task.first().toMap();

    QUrl url(editUrl);
    QNetworkRequest req(url);

    //TODO: Make more efficient by only uploading changes
    QString encodedData = QString("[{\"id\":" + taskMap["id"].toString() + "," +
                            "\"completed\":" + taskMap["completed"].toString() + "," +
                            "\"title\":\"" + taskMap["title"].toString() + "\"," +
                            "\"duedate\":\"" + taskMap["duedate"].toString() + "\"," +
                            "\"note\":\"" + taskMap["note"].toString() + "\"}]");
    encodedData = encodedData.replace("\n", "\\n").replace(" ", "+");
    encodedData = QUrl::toPercentEncoding(encodedData, "\"{}[]+\\,:", "");

    qDebug() << Q_FUNC_INFO << encodedData;

    QUrl data;
    data.addQueryItem("access_token", _propMan->accessToken);
    data.addEncodedQueryItem("tasks", encodedData.toAscii());
    data.addQueryItem("fields", "duedate,note");

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    _networkAccessManager->post(req, data.encodedQuery());
}

void TaskSenderReceiver::onTaskRemoved(QVariantList task) {
    //Task was removed by user in UI, need to upload changes
    QVariantMap taskMap = task.first().toMap();

    QUrl url(removeUrl);
    QNetworkRequest req(url);

    QUrl data;
    data.addQueryItem("access_token", _propMan->accessToken);
    data.addQueryItem("tasks", "[\"" + taskMap["id"].toString() + "\"]");

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    _networkAccessManager->post(req, data.encodedQuery());
}

void TaskSenderReceiver::onTasksReceived(QNetworkReply *reply) {
    QString response = reply->readAll();

    qDebug() << Q_FUNC_INFO << "Reply:" << response;

    bb::data::JsonDataAccess jda;
    QVariantList data = jda.loadFromBuffer(response).value<QVariantList>();
    if (jda.hasError()) {
        qDebug() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
    }

    if (reply->error() == QNetworkReply::NoError) {
        QVariantMap firstEntry = data.first().toMap();

        //If there's a summary entry, this is a list of all tasks
        if (firstEntry.contains("num") && firstEntry.contains("total")) {
            //Remove the summary item (gives number of tasks)
            data.pop_front();
            qDebug() << Q_FUNC_INFO << "Received tasks";
            emit tasksUpdated(data);
        }

        //If the ref field is taskAdd, then this is a new-added task reply
        if (firstEntry.contains("ref")) {
            if (firstEntry["ref"].toString() == "taskAdd") {
                qDebug() << Q_FUNC_INFO << "New task added";
                emit tasksAdded(data);
            }
        }

        //If there is just an id field, this is a reply to a delete call
        if (firstEntry.contains("id") && firstEntry.count() == 1) {
            qDebug() << Q_FUNC_INFO << "Task deleted" << firstEntry["id"];
            emit tasksRemoved(QVariantList() << firstEntry["id"]);
        }
    } else {
        //ToodleDo will come back with various error codes if there's a problem
        qDebug() << Q_FUNC_INFO << "Error code" << data.value(0) << "in network reply:" << data.value(1);
    }
    reply->deleteLater();
}

#include <bb/data/JsonDataAccess>

#include "TaskSenderReceiver.hpp"
#include "LoginManager.hpp"

const QString TaskSenderReceiver::getUrl = QString("http://api.toodledo.com/3/tasks/get.php");
const QString TaskSenderReceiver::editUrl = QString("http://api.toodledo.com/3/tasks/edit.php");
const QString TaskSenderReceiver::addUrl = QString("http://api.toodledo.com/3/tasks/add.php");

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
    url.addQueryItem("comp", QString::number(0));
    url.addEncodedQueryItem("fields", "duedate"); //id, title, modified, completed come automatically

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
    qDebug() << Q_FUNC_INFO << "Task added, sending to server";
    QVariantMap taskMap = task.first().toMap();
    qDebug() << taskMap;

    QUrl url(addUrl);
    QNetworkRequest req(url);

    QString taskData;
    bb::data::JsonDataAccess jda;
    jda.saveToBuffer(QVariant(taskMap), &taskData);

    qDebug() << taskData;

    QUrl data;
    data.addQueryItem("access_token", _propMan->accessToken);
    data.addEncodedQueryItem("tasks", taskData.toAscii());
    data.addQueryItem("fields", "");

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    _networkAccessManager->post(req, data.encodedQuery());
}

void TaskSenderReceiver::onTaskEdited(QVariantList task) {
    //Task was edited by user in UI, need to upload changes
    qDebug() << Q_FUNC_INFO << "Task edited, sending to server";
    qDebug() << task;
}

void TaskSenderReceiver::onTasksReceived(QNetworkReply *reply) {
    QString response = reply->readAll();

    bb::data::JsonDataAccess jda;
    QVariantList data = jda.loadFromBuffer(response).value<QVariantList>();
    if (jda.hasError()) {
        qDebug() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
    }

    if (reply->error() == QNetworkReply::NoError) {
        //All good!
        if (data.first().toMap().contains("num") && data.first().toMap().contains("total")) {
            //Remove the summary item (gives number of tasks)
            data.pop_front();
            qDebug() << Q_FUNC_INFO << "Received tasks";
            emit tasksUpdated(data);
        } else {
            qDebug() << Q_FUNC_INFO << "Received other info:" << data;
        }
    } else {
        //ToodleDo will come back with various error codes if there's a problem
        qDebug() << Q_FUNC_INFO << "Error code" << data.value(0) << "in network reply:" << data.value(1);
    }
    reply->deleteLater();
}

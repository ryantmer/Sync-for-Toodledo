#include <bb/data/JsonDataAccess>
#include "TaskSenderReceiver.hpp"

const QString TaskSenderReceiver::getUrl = QString("http://api.toodledo.com/3/tasks/get.php");
const QString TaskSenderReceiver::editUrl = QString("http://api.toodledo.com/3/tasks/edit.php");
const QString TaskSenderReceiver::addUrl = QString("http://api.toodledo.com/3/tasks/add.php");
const QString TaskSenderReceiver::removeUrl = QString("http://api.toodledo.com/3/tasks/delete.php");

using namespace bb::data;

TaskSenderReceiver::TaskSenderReceiver(QObject *parent) : QObject(parent) {
    _networkAccessManager = new QNetworkAccessManager(this);
    _propMan = PropertiesManager::getInstance();

    bool isOk;
    isOk = connect(_networkAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onReplyReceived(QNetworkReply*)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);
}

void TaskSenderReceiver::fetchAllTasks() {
    QUrl url(getUrl);
    url.addQueryItem("access_token", _propMan->accessToken);
    if (!_propMan->showCompletedTasks()) {
        url.addQueryItem("comp", 0); //only get incomplete tasks
    } else {
        url.addQueryItem("comp", QString::number(-1)); //get complete and incomplete
        url.addQueryItem("after", QString::number(
                QDateTime::currentDateTimeUtc().toTime_t() - 172800)); //tasks modified in past 48 hours
        url.addQueryItem("start", QString::number(0));
        url.addQueryItem("num", QString::number(30)); //only get first 30 tasks
    }
    url.addEncodedQueryItem("fields", "duedate,note,folder"); //id, title, modified, completed come automatically

    QNetworkRequest req(url);
    _networkAccessManager->get(req);
}

/*
 * Slots
 */
void TaskSenderReceiver::onTaskAdded(QVariantMap task) {
    QUrl url(addUrl);
    QNetworkRequest req(url);

    //Build task data string from user's input
    QString encodedData = QString("[{");
    if (task["title"].toString() != "") {
        encodedData.append("\"title\":\"" + task["title"].toString() + "\"");
    } else {
        //Can't add a task without a title
        //This should never be hit, as QML doesn't allow adding without a title
        qDebug() << Q_FUNC_INFO << "Can't add task without title! (you shouldn't ever see this)";
        return;
    }
    if (task["duedate"].toLongLong(NULL) > 0) {
        encodedData.append(",\"duedate\":" + task["duedate"].toString());
    }
    if (task["note"].toString() != "") {
        encodedData.append(",\"note\":\"" + task["note"].toString() + "\"");
    }
    if (task["folder"].toLongLong(NULL) > 0) {
        encodedData.append(",\"folder\":" + task["folder"].toString());
    }
    encodedData.append("}]");
    //Required for ToodleDo's API to replace some stuff
    encodedData = encodedData.replace("\n", "\\n").replace(" ", "+");
    encodedData = QUrl::toPercentEncoding(encodedData, "\"{}[]+\\,:", "");

    QUrl data;
    data.addQueryItem("access_token", _propMan->accessToken);
    data.addEncodedQueryItem("tasks", encodedData.toAscii());
    data.addQueryItem("fields", "duedate,note,folder");

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    _networkAccessManager->post(req, data.encodedQuery());
}

void TaskSenderReceiver::onTaskEdited(QVariantMap oldData, QVariantMap newData) {
    QUrl url(editUrl);
    QNetworkRequest req(url);

    if (oldData == newData) {
        //If nothing has changed, don't need to upload anything
        return;
    }

    //Build data string to only include fields that actually changed
    QString encodedData = QString("[{\"id\":" + newData["id"].toString());
    if (oldData["completed"] != newData["completed"]) {
        encodedData.append(",\"completed\":" + newData["completed"].toString());
    }
    if (oldData["title"] != newData["title"]) {
        encodedData.append(",\"title\":\"" + newData["title"].toString() + "\"");
    }
    if (oldData["duedate"] != newData["duedate"]) {
        encodedData.append(",\"duedate\":" + newData["duedate"].toString());
    }
    if (oldData["note"] != newData["note"]) {
        encodedData.append(",\"note\":\"" + newData["note"].toString() + "\"");
    }
    if (oldData["folder"] != newData["folder"]) {
        encodedData.append(",\"folder\":" + newData["folder"].toString());
    }
    encodedData.append("}]");
    encodedData = encodedData.replace("\n", "\\n").replace(" ", "+");
    encodedData = QUrl::toPercentEncoding(encodedData, "\"{}[]+\\,:", "");

    QUrl data;
    data.addQueryItem("access_token", _propMan->accessToken);
    data.addEncodedQueryItem("tasks", encodedData.toAscii());
    data.addQueryItem("fields", "duedate,note,folder");

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    _networkAccessManager->post(req, data.encodedQuery());
}

void TaskSenderReceiver::onTaskRemoved(QVariantMap task) {
    QUrl url(removeUrl);
    QNetworkRequest req(url);

    QUrl data;
    data.addQueryItem("access_token", _propMan->accessToken);
    data.addQueryItem("tasks", "[\"" + task["id"].toString() + "\"]");

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    _networkAccessManager->post(req, data.encodedQuery());
}

void TaskSenderReceiver::onReplyReceived(QNetworkReply *reply) {
    QString response = reply->readAll();

    JsonDataAccess jda;
    QVariantList data = jda.loadFromBuffer(response).value<QVariantList>();
    if (jda.hasError()) {
        qDebug() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
        qDebug() << Q_FUNC_INFO << response;
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        if (reply->url().toString().contains(getUrl)) {
            qDebug() << Q_FUNC_INFO << "Task(s) received";
            if (data.first().toMap().contains("num") && data.first().toMap().contains("total")) {
                data.pop_front();
            }
            for (int i = 0; i < data.count(); ++i) {
                emit taskGetReply(data.value(i).toMap());
            }
            emit toast("Tasks updated");
        } else if (reply->url().toString().contains(addUrl)) {
            qDebug() << Q_FUNC_INFO << "New task(s) added";
            for (int i = 0; i < data.count(); ++i) {
                emit taskAddReply(data.value(i).toMap());
            }
            emit toast("Task added");
        } else if (reply->url().toString().contains(removeUrl)) {
            qDebug() << Q_FUNC_INFO << "Task(s) removed";
            for (int i = 0; i < data.count(); ++i) {
                emit taskRemoveReply(data.value(i).toMap());
            }
            emit toast("Task deleted");
        } else if (reply->url().toString().contains(editUrl)) {
            qDebug() << Q_FUNC_INFO << "Task(s) edited";
            for (int i = 0; i < data.count(); ++i) {
                emit taskEditReply(data.value(i).toMap());
            }
            emit toast("Task edited");
        } else {
            qDebug() << Q_FUNC_INFO << "Unrecognized reply received:" << data;
        }
    } else {
        //ToodleDo will come back with various error codes if there's a problem
        QVariantMap errorMap = jda.loadFromBuffer(response).value<QVariantMap>();
        if (jda.hasError()) {
            qDebug() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
            qDebug() << Q_FUNC_INFO << response;
            return;
        }

        qDebug() << Q_FUNC_INFO << "ToodleDo error" <<
                errorMap.value("errorCode").toInt(NULL) << ":" <<
                errorMap.value("errorDesc").toString();
        emit toast("Toodledo Error " + errorMap.value("errorCode").toString() +
                    " : " + errorMap.value("errorDesc").toString());
    }
    reply->deleteLater();
}
/*
 * End slots
 */

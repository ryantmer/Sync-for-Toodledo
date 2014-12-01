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
    url.addQueryItem("comp", 0); //only get incomplete tasks
    //some fields (id, title, modified, completed) come automatically
    url.addEncodedQueryItem("fields", "duedate,note,folder,star,tag,priority,duetime,"
            "duedatemod,startdate,starttime,remind,repeat,status,length");

    QNetworkRequest req(url);
    _networkAccessManager->get(req);
}

/*
 * Slots
 */
void TaskSenderReceiver::onTaskAdded(QVariantMap task) {
    QUrl url(addUrl);
    QNetworkRequest req(url);

    if (task["title"].toString() == "") {
        //Can't add a task without a title
        //This should never be hit, as QML doesn't allow adding without a title
        qCritical() << Q_FUNC_INFO << "Can't add task without title! (you shouldn't ever see this)";
        return;
    }

    //Build task data string from user's input
    QString encodedData = QString("[{");
    QString returnFields = "folder,tag,startdate,duedate,duedatemod,starttime,"
                "duetime,remind,repeat,status,star,priority,length,note";
    QVariantMap::iterator i;
    for (i = task.begin(); i != task.end(); ++i) {
        bool isOk;
        int value = task[i.key()].toInt(&isOk);
        Q_UNUSED(value);
        if (isOk) {
            //number values
            encodedData.append(",\"" + i.key() + "\":" + task[i.key()].toString());
        } else {
            //string values (extra quotation marks needed)
            encodedData.append(",\"" + i.key() + "\":\"" + task[i.key()].toString() + "\"");
        }
    }
    encodedData.append("}]");
    encodedData.remove(2, 1);  //Remove initial comma YES THIS IS MESSY I KNOW STOP YELLING.
    qDebug() << Q_FUNC_INFO << encodedData;
    //Required for ToodleDo's API to replace some stuff
    encodedData = encodedData.replace("\n", "\\n").replace(" ", "+");
    encodedData = QUrl::toPercentEncoding(encodedData, "\"{}[]+\\,:", "");

    QUrl data;
    data.addQueryItem("access_token", _propMan->accessToken);
    data.addEncodedQueryItem("tasks", encodedData.toAscii());
    data.addQueryItem("fields", returnFields);

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
    QString encodedData = QString("[{");
    encodedData.append("\"id\":" + oldData["id"].toString());
    QString returnFields = "folder,tag,startdate,duedate,duedatemod,starttime,"
                "duetime,remind,repeat,status,star,priority,length,note";
    QVariantMap::iterator i;
    for (i = newData.begin(); i != newData.end(); ++i) {
        if (newData[i.key()] != oldData[i.key()]) {
            bool isOk;
            int value = newData[i.key()].toInt(&isOk);
            Q_UNUSED(value);
            if (isOk) {
                //number values
                encodedData.append(",\"" + i.key() + "\":" + newData[i.key()].toString());
            } else {
                //string values (extra quotation marks needed)
                encodedData.append(",\"" + i.key() + "\":\"" + newData[i.key()].toString() + "\"");
            }
        }
    }
    encodedData.append("}]");
    qDebug() << Q_FUNC_INFO << encodedData;
    encodedData = encodedData.replace("\n", "\\n").replace(" ", "+");
    encodedData = QUrl::toPercentEncoding(encodedData, "\"{}[]+\\,:", "");

    QUrl data;
    data.addQueryItem("access_token", _propMan->accessToken);
    data.addEncodedQueryItem("tasks", encodedData.toAscii());
    data.addQueryItem("fields", returnFields);

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

    qDebug() << Q_FUNC_INFO << response;

    JsonDataAccess jda;
    QVariantList data = jda.loadFromBuffer(response).value<QVariantList>();
    if (jda.hasError()) {
        qWarning() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
        qWarning() << Q_FUNC_INFO << response;
        return;
    }

    QVariantMap first = data.first().toMap();
    if (first.contains("errorCode")) {
        qWarning() << Q_FUNC_INFO << "Toodledo error" <<
                first.value("errorCode").toInt(NULL) << ":" <<
                first.value("errorDesc").toString();
        emit toast("Toodledo Error " + first.value("errorCode").toString() +
                    " : " + first.value("errorDesc").toString());
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        if (reply->url().toString().contains(getUrl)) {
            qDebug() << Q_FUNC_INFO << "Task(s) received";
            //First item is a summary item, discard it
            if (data.first().toMap().contains("num") && data.first().toMap().contains("total")) {
                data.pop_front();
            }
            for (int i = 0; i < data.count(); ++i) {
                emit taskGetReply(data.value(i).toMap());
            }
            emit toast("Tasks received");
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
            qDebug() << Q_FUNC_INFO << "Task(s) updated";
            for (int i = 0; i < data.count(); ++i) {
                emit taskEditReply(data.value(i).toMap());
            }
            emit toast("Task updated");
        } else {
            qWarning() << Q_FUNC_INFO << "Unrecognized reply received:" << data;
        }
    } else {
        qWarning() << Q_FUNC_INFO << "Network error";
        emit toast("Network error!");
    }
    reply->deleteLater();
}
/*
 * End slots
 */

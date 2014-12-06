#include <bb/data/JsonDataAccess>
#include "SenderReceiver.hpp"

const QString SenderReceiver::taskGetUrl = QString("http://api.toodledo.com/3/tasks/get.php");
const QString SenderReceiver::taskEditUrl = QString("http://api.toodledo.com/3/tasks/edit.php");
const QString SenderReceiver::taskAddUrl = QString("http://api.toodledo.com/3/tasks/add.php");
const QString SenderReceiver::taskRemoveUrl = QString("http://api.toodledo.com/3/tasks/delete.php");
const QString SenderReceiver::folderGetUrl = QString("http://api.toodledo.com/3/folders/get.php");
const QString SenderReceiver::folderEditUrl = QString("http://api.toodledo.com/3/folders/edit.php");
const QString SenderReceiver::folderAddUrl = QString("http://api.toodledo.com/3/folders/add.php");
const QString SenderReceiver::folderRemoveUrl = QString("http://api.toodledo.com/3/folders/delete.php");

using namespace bb::data;

SenderReceiver::SenderReceiver(QObject *parent) : QObject(parent) {
    _networkAccessManager = new QNetworkAccessManager(this);
    _propMan = PropertiesManager::getInstance();

    bool isOk;
    isOk = connect(_networkAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onReplyReceived(QNetworkReply*)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);
}

void SenderReceiver::fetchAllTasks() {
    QUrl url(taskGetUrl);
    url.addQueryItem("access_token", _propMan->accessToken);
    url.addQueryItem("comp", 0); //only get incomplete tasks
    //some fields (id, title, modified, completed) come automatically
    url.addEncodedQueryItem("fields", "duedate,note,folder,star,tag,priority,duetime,"
            "duedatemod,startdate,starttime,remind,repeat,status,length");

    QNetworkRequest req(url);
    _networkAccessManager->get(req);
}

void SenderReceiver::fetchAllFolders() {
    QUrl url(folderGetUrl);
    url.addQueryItem("access_token", _propMan->accessToken);

    QNetworkRequest req(url);
    _networkAccessManager->get(req);
}

/*
 * Slots
 */
void SenderReceiver::onTaskAdded(QVariantMap task) {
    QUrl url(taskAddUrl);
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

void SenderReceiver::onTaskEdited(QVariantMap oldData, QVariantMap newData) {
    QUrl url(taskEditUrl);
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

void SenderReceiver::onTaskRemoved(QVariantMap task) {
    QUrl url(taskRemoveUrl);
    QNetworkRequest req(url);

    QUrl data;
    data.addQueryItem("access_token", _propMan->accessToken);
    data.addQueryItem("tasks", "[\"" + task["id"].toString() + "\"]");

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    _networkAccessManager->post(req, data.encodedQuery());
}

void SenderReceiver::onFolderAdded(QVariantMap folder) {
    QUrl url(folderAddUrl);
    QNetworkRequest req(url);

    QUrl data;
    data.addQueryItem("access_token", _propMan->accessToken);
    if (folder["name"].toString() != "") {
        data.addQueryItem("name", folder["name"].toString());
    } else {
        //Can't add a folder without a name
        //This should never be hit, as QML doesn't allow adding without a name
        qCritical() << Q_FUNC_INFO << "Can't add folder without name! (you shouldn't ever see this)";
        return;
    }
    if (folder["private"].toInt(NULL) == 1) {
        data.addQueryItem("private", folder["private"].toString());
    }

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    _networkAccessManager->post(req, data.encodedQuery());
}

void SenderReceiver::onFolderEdited(QVariantMap oldData, QVariantMap newData) {
    QUrl url(folderEditUrl);
    QNetworkRequest req(url);

    if (oldData == newData) {
        //If nothing has changed, don't need to upload anything
        return;
    }

    QUrl data;
    data.addQueryItem("access_token", _propMan->accessToken);
    data.addQueryItem("id", newData["id"].toString());
    QVariantMap::iterator i;
    for (i = newData.begin(); i != newData.end(); ++i) {
        if (oldData[i.key()] != newData[i.key()]) {
            data.addQueryItem(i.key(), newData[i.key()].toString());
        }
    }

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    _networkAccessManager->post(req, data.encodedQuery());
}

void SenderReceiver::onFolderRemoved(QVariantMap folder) {
    QUrl url(folderRemoveUrl);
    QNetworkRequest req(url);

    QUrl data;
    data.addQueryItem("access_token", _propMan->accessToken);
    data.addQueryItem("id", folder["id"].toString());

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    _networkAccessManager->post(req, data.encodedQuery());
}

void SenderReceiver::onReplyReceived(QNetworkReply *reply) {
    QString response = reply->readAll();

    if (reply->error() == QNetworkReply::NoError) {
        /* Reply handling here looks waaaaay too complicated, but it's because
         * replies come in one of three formats: a single map, a list of maps
         * with only one entry, or a list of maps with multiple entries. Way
         * to be consistent, Toodledo API...
         */
        JsonDataAccess jda;
        QVariantList dataList = jda.loadFromBuffer(response).value<QVariantList>();
        if (jda.hasError()) {
            qWarning() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
            qWarning() << Q_FUNC_INFO << response;
            return;
        }

        QVariantMap dataMap = jda.loadFromBuffer(response).value<QVariantMap>();
        if (jda.hasError()) {
            qWarning() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
            qWarning() << Q_FUNC_INFO << response;
            return;
        }

        if (dataMap.contains("errorDesc")) {
            qWarning() << Q_FUNC_INFO << "Toodledo error" <<
                        dataMap.value("errorCode").toInt(NULL) << ":" <<
                        dataMap.value("errorDesc").toString();
            emit toast("Toodledo Error " + dataMap.value("errorCode").toString() +
                        " : " + dataMap.value("errorDesc").toString());
            return;
        }

        if (reply->url().toString().contains(taskGetUrl)) {
            qDebug() << Q_FUNC_INFO << "Task(s) received";
            //First item is a summary item, discard it
            if (dataList.first().toMap().contains("num") && dataList.first().toMap().contains("total")) {
                dataList.pop_front();
            }
            for (int i = 0; i < dataList.count(); ++i) {
                emit taskGetReply(dataList.value(i).toMap());
            }
            emit toast("Tasks received");
        } else if (reply->url().toString().contains(taskAddUrl)) {
            qDebug() << Q_FUNC_INFO << "New task(s) added";
            for (int i = 0; i < dataList.count(); ++i) {
                emit taskAddReply(dataList.value(i).toMap());
            }
            emit toast("Task added");
        } else if (reply->url().toString().contains(taskRemoveUrl)) {
            qDebug() << Q_FUNC_INFO << "Task(s) removed";
            for (int i = 0; i < dataList.count(); ++i) {
                emit taskRemoveReply(dataList.value(i).toMap());
            }
            emit toast("Task deleted");
        } else if (reply->url().toString().contains(taskEditUrl)) {
            qDebug() << Q_FUNC_INFO << "Task(s) updated";
            for (int i = 0; i < dataList.count(); ++i) {
                emit taskEditReply(dataList.value(i).toMap());
            }
            emit toast("Task updated");
        } else if (reply->url().toString().contains(folderGetUrl)) {
            qDebug() << Q_FUNC_INFO << "Folder(s) received";
            for (int i = 0; i < dataList.count(); ++i) {
                emit folderGetReply(dataList.value(i).toMap());
            }
        } else if (reply->url().toString().contains(folderAddUrl)) {
            qDebug() << Q_FUNC_INFO << "New folder(s) added";
            for (int i = 0; i < dataList.count(); ++ i) {
                emit folderAddReply(dataList.value(i).toMap());
            }
        } else if (reply->url().toString().contains(folderRemoveUrl)) {
            qDebug() << Q_FUNC_INFO << "Folder(s) removed";
            emit folderRemoveReply(dataMap);
        } else if (reply->url().toString().contains(folderEditUrl)) {
            qDebug() << Q_FUNC_INFO << "Folder(s) edited";
            for (int i = 0; i < dataList.count(); ++i) {
                emit folderEditReply(dataList.value(i).toMap());
            }
        } else {
            qWarning() << Q_FUNC_INFO << "Unrecognized reply received:" << response;
        }
    } else {
        qWarning() << Q_FUNC_INFO << "Network error";
        qWarning() << Q_FUNC_INFO << response;
        emit toast("Network error!");
    }
    reply->deleteLater();
}
/*
 * End slots
 */

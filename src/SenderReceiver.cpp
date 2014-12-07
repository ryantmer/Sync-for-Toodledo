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

SenderReceiver::~SenderReceiver() {}

void SenderReceiver::setDataType(DataType dataType) {
    _dataType = dataType;
}

void SenderReceiver::fetchAll() {
    QUrl url;
    switch (_dataType) {
        case Task:
            url.setUrl(taskGetUrl);
            url.addQueryItem("comp", 0); //only get incomplete tasks
            //some fields (id, title, modified, completed) come automatically
            url.addEncodedQueryItem("fields", "duedate,note,folder,star,tag,priority,duetime,"
                    "duedatemod,startdate,starttime,remind,repeat,status,length");
            break;
        case Folder:
            url.setUrl(folderGetUrl);
            break;
    }

    url.addQueryItem("access_token", _propMan->accessToken);

    QNetworkRequest req(url);
    _networkAccessManager->get(req);
}

/*
 * Slots
 */
void SenderReceiver::onAdd(QVariantMap data) {
    QUrl url;
    QUrl urlData;
    QVariantMap::iterator iter;

    switch (_dataType) {
        case Task: {
            url.setUrl(taskAddUrl);
            //Build task data string from user's input
            QString encodedData = QString("[{");
            QString returnFields = "folder,tag,startdate,duedate,duedatemod,starttime,"
                        "duetime,remind,repeat,status,star,priority,length,note";
            for (iter = data.begin(); iter != data.end(); ++iter) {
                bool isOk;
                int value = data[iter.key()].toInt(&isOk);
                Q_UNUSED(value);
                if (isOk) {
                    //number values
                    encodedData.append(",\"" + iter.key() + "\":" + iter.value().toString());
                } else {
                    //string values (extra quotation marks needed)
                    encodedData.append(",\"" + iter.key() + "\":\"" + iter.value().toString() + "\"");
                }
            }
            encodedData.append("}]");
            encodedData.remove(2, 1);  //Remove initial comma YES THIS IS MESSY I KNOW STOP YELLING.
            qDebug() << Q_FUNC_INFO << encodedData;
            //Required for ToodleDo's API to replace some stuff
            encodedData = encodedData.replace("\n", "\\n").replace(" ", "+");
            encodedData = QUrl::toPercentEncoding(encodedData, "\"{}[]+\\,:", "");

            urlData.addEncodedQueryItem("tasks", encodedData.toAscii());
            urlData.addQueryItem("fields", returnFields);
            break;
        }
        case Folder: {
            url.setUrl(folderAddUrl);
            for (iter = data.begin(); iter != data.end(); ++iter) {
                urlData.addQueryItem(iter.key(), data[iter.key()].toString());
            }
            break;
        }
    }

    urlData.addQueryItem("access_token", _propMan->accessToken);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    _networkAccessManager->post(req, urlData.encodedQuery());
}

void SenderReceiver::onEdit(QVariantMap oldData, QVariantMap newData) {
    if (oldData == newData) {
        //If nothing has changed, don't need to upload anything
        return;
    }

    QUrl url;
    QUrl urlData;
    QVariantMap::iterator iter;

    switch (_dataType) {
        case Task: {
            url.setUrl(taskEditUrl);
            QString encodedData = QString("[{");
            encodedData.append("\"id\":" + oldData["id"].toString());
            QString returnFields = "folder,tag,startdate,duedate,duedatemod,starttime,"
                        "duetime,remind,repeat,status,star,priority,length,note";
            for (iter = newData.begin(); iter != newData.end(); ++iter) {
                if (newData[iter.key()] != oldData[iter.key()]) {
                    bool isOk;
                    int value = newData[iter.key()].toInt(&isOk);
                    Q_UNUSED(value);
                    if (isOk) {
                        //number values
                        encodedData.append(",\"" + iter.key() + "\":" + iter.value().toString());
                    } else {
                        //string values (extra quotation marks needed)
                        encodedData.append(",\"" + iter.key() + "\":\"" + iter.value().toString() + "\"");
                    }
                }
            }
            encodedData.append("}]");
            encodedData = encodedData.replace("\n", "\\n").replace(" ", "+");
            encodedData = QUrl::toPercentEncoding(encodedData, "\"{}[]+\\,:", "");
            urlData.addEncodedQueryItem("tasks", encodedData.toAscii());
            urlData.addQueryItem("fields", returnFields);
            break;
        }
        case Folder: {
            url.setUrl(folderEditUrl);
            urlData.addQueryItem("id", newData["id"].toString());
            for (iter = newData.begin(); iter != newData.end(); ++iter) {
                if (oldData[iter.key()] != newData[iter.key()]) {
                    urlData.addQueryItem(iter.key(), iter.value().toString());
                }
            }
            break;
        }
    }

    urlData.addQueryItem("access_token", _propMan->accessToken);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    _networkAccessManager->post(req, urlData.encodedQuery());
}

void SenderReceiver::onRemove(QVariantMap data) {
    QUrl url;
    QUrl urlData;

    switch (_dataType) {
        case Task:
            url.setUrl(taskRemoveUrl);
            urlData.addQueryItem("tasks", "[\"" + data["id"].toString() + "\"]");
            break;
        case Folder:
            url.setUrl(folderRemoveUrl);
            urlData.addQueryItem("id", data["id"].toString());
            break;
    }

    urlData.addQueryItem("access_token", _propMan->accessToken);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    _networkAccessManager->post(req, urlData.encodedQuery());

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

        QString replyUrl = reply->url().toString(QUrl::RemoveQuery);
        if (replyUrl == taskGetUrl) {
            qDebug() << Q_FUNC_INFO << "Task(s) received";
            //First item is a summary item, discard it
            if (dataList.first().toMap().contains("num") && dataList.first().toMap().contains("total")) {
                dataList.pop_front();
            }
            for (int i = 0; i < dataList.count(); ++i) {
                emit taskGetReply(dataList.value(i).toMap());
            }
            emit toast("Tasks received");
        } else if (replyUrl == taskAddUrl) {
            qDebug() << Q_FUNC_INFO << "New task(s) added";
            for (int i = 0; i < dataList.count(); ++i) {
                emit taskAddReply(dataList.value(i).toMap());
            }
        } else if (replyUrl == taskRemoveUrl) {
            qDebug() << Q_FUNC_INFO << "Task(s) removed";
            for (int i = 0; i < dataList.count(); ++i) {
                emit taskRemoveReply(dataList.value(i).toMap());
            }
        } else if (replyUrl == taskEditUrl) {
            qDebug() << Q_FUNC_INFO << "Task(s) updated";
            for (int i = 0; i < dataList.count(); ++i) {
                emit taskEditReply(dataList.value(i).toMap());
            }
        } else if (replyUrl == folderGetUrl) {
            qDebug() << Q_FUNC_INFO << "Folder(s) received";
            for (int i = 0; i < dataList.count(); ++i) {
                emit folderGetReply(dataList.value(i).toMap());
            }
        } else if (replyUrl == folderAddUrl) {
            qDebug() << Q_FUNC_INFO << "New folder(s) added";
            for (int i = 0; i < dataList.count(); ++ i) {
                emit folderAddReply(dataList.value(i).toMap());
            }
        } else if (replyUrl == folderRemoveUrl) {
            qDebug() << Q_FUNC_INFO << "Folder(s) removed";
            emit folderRemoveReply(dataMap);
        } else if (replyUrl == folderEditUrl) {
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

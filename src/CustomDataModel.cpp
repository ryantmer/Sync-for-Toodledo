#include "CustomDataModel.hpp"
#include <bb/data/JsonDataAccess>

const QString CustomDataModel::getUrl = QString("http://api.toodledo.com/3/%1/get.php");
const QString CustomDataModel::addUrl = QString("http://api.toodledo.com/3/%1/add.php");
const QString CustomDataModel::editUrl = QString("http://api.toodledo.com/3/%1/edit.php");
const QString CustomDataModel::removeUrl = QString("http://api.toodledo.com/3/%1/delete.php");
const QString CustomDataModel::tasks = QString("tasks");
const QString CustomDataModel::folders = QString("folders");
const QString CustomDataModel::contexts = QString("contexts");
const QString CustomDataModel::goals = QString("goals");
const QString CustomDataModel::locations = QString("locations");

using namespace bb::cascades;
using namespace bb::data;

CustomDataModel::CustomDataModel(QObject *parent) : DataModel(parent) {
    _dataType = UndefinedType;

    _networkAccessManager = new QNetworkAccessManager(this);
    _propMan = PropertiesManager::getInstance();
    _netMan = NetworkManager::getInstance();
    _loginMan = LoginManager::getInstance();

    //Connect signals
    bool isOk;
    isOk = connect(_networkAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onReplyReceived(QNetworkReply*)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);
}

CustomDataModel::~CustomDataModel() {}

void CustomDataModel::setDataType(DataType dataType) {
    _dataType = dataType;
}

void CustomDataModel::refresh() {
    if (_netMan->isConnected()) {
        if (_loginMan->isLoggedIn()) {
            clear();
            populateDataModel();
        } else {
            qWarning() << Q_FUNC_INFO << "LoginManager indicated not logged in";
        }
    } else {
        qWarning() << Q_FUNC_INFO << "NetworkManager indicated no network connection";
        emit toast("No network connection!");
    }
}

void CustomDataModel::populateDataModel() {
    //Gets data from API based on _dataType.
    QUrl url;
    QUrl urlData;

    switch (_dataType) {
        case Task:
            url.setUrl(getUrl.arg(tasks));
            urlData.addQueryItem("comp", QString::number(0)); //incomplete tasks
            //fields id, title, modified, completed come automatically
            urlData.addEncodedQueryItem("fields", "duedate,note,folder,star,tag,priority,duetime,"
                    "duedatemod,startdate,starttime,remind,repeat,status,length");
            break;
        case Folder:
            url.setUrl(getUrl.arg(folders));
            break;
        case CompletedTask:
            url.setUrl(getUrl.arg(tasks));
            //only get tasks modified in last 24 hours
            urlData.addQueryItem("before", QString::number(QDateTime::currentDateTimeUtc().toTime_t()));
            urlData.addQueryItem("after", QString::number(QDateTime::currentDateTimeUtc().toTime_t() - 86400));
            urlData.addQueryItem("comp", QString::number(1));
            urlData.addQueryItem("fields", "note");
            break;
        case Context:
            url.setUrl(getUrl.arg(contexts));
            break;
        case Goal:
            url.setUrl(getUrl.arg(goals));
            break;
        case Location:
        default:
            break;
    }

    urlData.addQueryItem("access_token", _propMan->accessToken);
    qDebug() << Q_FUNC_INFO << url;
    qDebug() << Q_FUNC_INFO << urlData;
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    _networkAccessManager->post(req, urlData.encodedQuery());
}

void CustomDataModel::clear() {
    _internalDB.clear();
}

QVariantList CustomDataModel::getInternalList() {
    return _internalDB;
}

bool compareTasks(QVariant &a, QVariant &b) {
    QVariantMap first = a.toMap();
    QVariantMap second = b.toMap();

    //duedate=0 means no due date; these are sent to the end/bottom
    if (first.value("duedate").toLongLong(NULL) == 0) {
        return false;
    } else if (second.value("duedate").toLongLong(NULL) == 0) {
        return true;
    } else if (first.value("duedate").toLongLong(NULL) == second.value("duedate").toLongLong(NULL)) {
        //If two tasks has the same duedate timestamp, order by modified timestamp
        return first.value("modified").toLongLong(NULL) >= second.value("modified").toLongLong(NULL);
    } else {
        return first.value("duedate").toLongLong(NULL) <= second.value("duedate").toLongLong(NULL);
    }
}

bool compareFolders(QVariant &a, QVariant &b) {
    QVariantMap first = a.toMap();
    QVariantMap second = b.toMap();

    if (first.value("archived").toLongLong(NULL) == 0 && second.value("archived").toLongLong(NULL) != 0) {
        return true;
    } else if (first.value("archived").toLongLong(NULL) != 0 && second.value("archived").toLongLong(NULL) == 0) {
        return false;
    } else {
        return first.value("ord").toLongLong(NULL) < second.value("ord").toLongLong(NULL);
    }
}

bool compareCompletedTasks(QVariant &a, QVariant &b) {
    QVariantMap first = a.toMap();
    QVariantMap second = b.toMap();

    if (first.value("completed").toLongLong(NULL) == second.value("completed").toLongLong(NULL)) {
        return first.value("modified").toLongLong(NULL) > second.value("modified").toLongLong(NULL);
    } else {
        return first.value("completed").toLongLong(NULL) > second.value("completed").toLongLong(NULL);
    }
}

bool compareContexts(QVariant &a, QVariant &b) {
    QVariantMap first = a.toMap();
    QVariantMap second = b.toMap();

    return first.value("private").toLongLong(NULL) <= second.value("private").toLongLong(NULL);
}

bool compareGoals(QVariant &a, QVariant &b) {
    QVariantMap first = a.toMap();
    QVariantMap second = b.toMap();

    return first.value("level").toLongLong(NULL) <= second.value("level").toLongLong(NULL);
}

void CustomDataModel::sort() {
    switch (_dataType) {
        case Task:
            qSort(_internalDB.begin(), _internalDB.end(), compareTasks);
            break;
        case Folder:
            qSort(_internalDB.begin(), _internalDB.end(), compareFolders);
            break;
        case CompletedTask:
            qSort(_internalDB.begin(), _internalDB.end(), compareCompletedTasks);
            break;
        case Context:
            qSort(_internalDB.begin(), _internalDB.end(), compareContexts);
            break;
        case Goal:
            qSort(_internalDB.begin(), _internalDB.end(), compareGoals);
            break;
        case Location:
        default:
            break;
    }
}

void CustomDataModel::addToDataModel(QVariantMap data) {
    _internalDB.append(data);
    sort();
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
    qDebug() << Q_FUNC_INFO << "New data added to CustomDataModel:" << data;
}

void CustomDataModel::editInDataModel(QVariantMap data) {
    qlonglong editId = data.value("id").toLongLong(NULL);
    for (int i = 0; i < _internalDB.count(); ++i) {
        qlonglong currentId = _internalDB.value(i).toMap().value("id").toLongLong(NULL);
        if (currentId == editId) {
            //Create a new QVM, overwriting pre-existing values, and inserting new ones if required
            QVariantMap amalgamatedData = _internalDB.value(i).toMap();
            for (QVariantMap::const_iterator iter = data.begin(); iter != data.end(); ++iter) {
                amalgamatedData.insert(iter.key(), iter.value());
            }
            _internalDB.replace(i, amalgamatedData);

            if (_dataType == Task) {
                //Special case for a task that is set to complete; remove from CDM
                if (_internalDB.value(i).toMap().value("completed").toLongLong(NULL) != 0) {
                    _internalDB.removeAt(i);
                }
            } else if (_dataType == CompletedTask) {
                //Special case for a completed task that is set to not complete; remove from CDM
                if (_internalDB.value(i).toMap().value("completed").toLongLong(NULL) == 0) {
                    _internalDB.removeAt(i);
                }
            }

            sort();
            emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
            qDebug() << Q_FUNC_INFO << "Data edited in CustomDataModel:" << data;
            return;
        }
    }

    //If not found in datamodel, add as new item
    addToDataModel(data);
}

void CustomDataModel::removeFromDataModel(QVariantMap data) {
    qDebug() << Q_FUNC_INFO << data;
    qlonglong removeId;
    switch (_dataType) {
        case Task:
        case CompletedTask:
            removeId = data.value("id").toLongLong(NULL);
            break;
        case Folder:
        case Context:
        case Goal:
            removeId = data.value("deleted").toLongLong(NULL);
            break;
        case Location:
        default:
            break;
    }

    for (int i = 0; i < _internalDB.count(); ++i) {
        qlonglong currentId = _internalDB.value(i).toMap().value("id").toLongLong(NULL);
        if (currentId == removeId) {
            _internalDB.removeAt(i);
            sort();
            emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
            qDebug() << Q_FUNC_INFO << "Data removed from CustomDataModel:" << data;
            break;
        }
    }
}

void CustomDataModel::add(QVariantMap data) {
    QUrl url;
    QUrl urlData;
    QVariantMap::iterator iter;

    switch (_dataType) {
        case Task:
        case CompletedTask: {
            url.setUrl(addUrl.arg(tasks));
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
        case Folder:
            url.setUrl(addUrl.arg(folders));
            for (iter = data.begin(); iter != data.end(); ++iter) {
                urlData.addQueryItem(iter.key(), data[iter.key()].toString());
            }
            break;
        case Context:
            url.setUrl(addUrl.arg(contexts));
            for (iter = data.begin(); iter != data.end(); ++iter) {
                urlData.addQueryItem(iter.key(), data[iter.key()].toString());
            }
            break;
        case Goal:
            url.setUrl(addUrl.arg(goals));
            for (iter = data.begin(); iter != data.end(); ++iter) {
                urlData.addQueryItem(iter.key(), data[iter.key()].toString());
            }
            break;
        case Location:
        default:
            break;
    }

    urlData.addQueryItem("access_token", _propMan->accessToken);
    qDebug() << Q_FUNC_INFO << url;
    qDebug() << Q_FUNC_INFO << urlData;
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    _networkAccessManager->post(req, urlData.encodedQuery());
}

void CustomDataModel::edit(QVariantMap oldData, QVariantMap newData) {
    if (oldData == newData) {
        //If nothing has changed, don't need to upload anything
        return;
    }

    QUrl url;
    QUrl urlData;
    QVariantMap::iterator iter;

    switch (_dataType) {
        case Task:
        case CompletedTask: {
            url.setUrl(editUrl.arg(tasks));
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
        case Folder:
            url.setUrl(editUrl.arg(folders));
            urlData.addQueryItem("id", newData["id"].toString());
            for (iter = newData.begin(); iter != newData.end(); ++iter) {
                if (oldData[iter.key()] != newData[iter.key()]) {
                    urlData.addQueryItem(iter.key(), iter.value().toString());
                }
            }
            break;
        case Context:
            url.setUrl(editUrl.arg(contexts));
            urlData.addQueryItem("id", newData["id"].toString());
            for (iter = newData.begin(); iter != newData.end(); ++iter) {
                urlData.addQueryItem(iter.key(), iter.value().toString());
            }
            break;
        case Goal:
            url.setUrl(editUrl.arg(goals));
            urlData.addQueryItem("id", newData["id"].toString());
            for (iter = newData.begin(); iter != newData.end(); ++iter) {
                if (oldData[iter.key()] != newData[iter.key()]) {
                    urlData.addQueryItem(iter.key(), iter.value().toString());
                }
            }
            break;
        case Location:
        default:
            break;
    }

    urlData.addQueryItem("access_token", _propMan->accessToken);
    qDebug() << Q_FUNC_INFO << url;
    qDebug() << Q_FUNC_INFO << urlData;
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    _networkAccessManager->post(req, urlData.encodedQuery());
}

void CustomDataModel::remove(QVariantMap data) {
    QUrl url;
    QUrl urlData;

    switch (_dataType) {
        case Task:
        case CompletedTask:
            url.setUrl(removeUrl.arg(tasks));
            urlData.addQueryItem("tasks", "[\"" + data["id"].toString() + "\"]");
            break;
        case Folder:
            url.setUrl(removeUrl.arg(folders));
            urlData.addQueryItem("id", data["id"].toString());
            break;
        case Context:
            url.setUrl(removeUrl.arg(contexts));
            urlData.addQueryItem("id", data["id"].toString());
            break;
        case Goal:
            url.setUrl(removeUrl.arg(goals));
            urlData.addQueryItem("id", data["id"].toString());
            break;
        case Location:
        default:
            break;
    }

    urlData.addQueryItem("access_token", _propMan->accessToken);
    qDebug() << Q_FUNC_INFO << url;
    qDebug() << Q_FUNC_INFO << urlData;
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    _networkAccessManager->post(req, urlData.encodedQuery());
}

/*
 * Slots
 */
void CustomDataModel::onReplyReceived(QNetworkReply *reply) {
    QString response = reply->readAll();

    qDebug() << Q_FUNC_INFO << response;

    if (reply->error() == QNetworkReply::NoError) {
        JsonDataAccess jda;
        QVariantList dataList = jda.loadFromBuffer(response).value<QVariantList>();
        if (jda.hasError()) {
            qWarning() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
            qWarning() << Q_FUNC_INFO << response;
            return;
        }

        QString replyUrl = reply->url().toString(QUrl::RemoveQuery);

        if (replyUrl == getUrl.arg(tasks) || replyUrl == getUrl.arg(folders) ||
                replyUrl == getUrl.arg(contexts) || replyUrl == getUrl.arg(goals)) {
            qDebug() << Q_FUNC_INFO << "Get URL data received from" << replyUrl;
            //Discard summary item (only when getting tasks)
            if (dataList.length() > 0) {
                if (dataList.first().toMap().contains("num") && dataList.first().toMap().contains("total")) {
                    dataList.pop_front();
                }
            }
            for (int i = 0; i < dataList.count(); ++i) {
                addToDataModel(dataList.value(i).toMap());
            }
        }
        else if (replyUrl == addUrl.arg(tasks) || replyUrl == addUrl.arg(folders) ||
                replyUrl == addUrl.arg(contexts) || replyUrl == addUrl.arg(goals)) {
            qDebug() << Q_FUNC_INFO << "Add URL data received from" << replyUrl;
            for (int i = 0; i < dataList.count(); ++i) {
                addToDataModel(dataList.value(i).toMap());
            }
        }
        else if (replyUrl == editUrl.arg(tasks) || replyUrl == editUrl.arg(folders) ||
                replyUrl == editUrl.arg(contexts) || replyUrl == editUrl.arg(goals)) {
            qDebug() << Q_FUNC_INFO << "Edit URL data received from" << replyUrl;
            for (int i = 0; i < dataList.count(); ++i) {
                editInDataModel(dataList.value(i).toMap());
            }
        }
        else if (replyUrl == removeUrl.arg(tasks)) {
            qDebug() << Q_FUNC_INFO << "Remove URL data received from" << replyUrl;
            for (int i = 0; i < dataList.count(); ++i) {
                removeFromDataModel(dataList.value(i).toMap());
            }
        } else if (replyUrl == removeUrl.arg(folders) || replyUrl == removeUrl.arg(contexts) ||
                replyUrl == removeUrl.arg(goals)) {
            //These remove replies come as a single map, not list of maps
            qDebug() << Q_FUNC_INFO << "Remove URL data received from" << replyUrl;
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
            removeFromDataModel(dataMap);
        }
        else {
            qWarning() << Q_FUNC_INFO << "Unrecognized reply received from" << replyUrl;
            qWarning() << Q_FUNC_INFO << response;
        }
    } else {
        qWarning() << Q_FUNC_INFO << "Network error";
        qWarning() << Q_FUNC_INFO << response;
        emit toast("Network error!");
    }
    reply->deleteLater();
}

void CustomDataModel::onLogOut() {
    //Clear all stored tasks
    _internalDB = QVariantList();
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
}
/*
 * End slots
 */

/*
 * Functions required by DataModel. Not sure if these work.
 */
int CustomDataModel::childCount(const QVariantList &indexPath) {
    int retVal = 0;
    if (indexPath.length() == 0) {
        retVal = _internalDB.length();
    } else if (indexPath.length() == 1) {
        //Only a task's attachment property has children
        QVariantMap map = _internalDB.value(indexPath.value(0).toInt(NULL)).toMap();
        if (map.contains("attachment")) {
            retVal = map["attachment"].toList().length();
        }
    }
    return retVal;
}
bool CustomDataModel::hasChildren(const QVariantList &indexPath) {
    bool retVal = false;
    if (indexPath.length() == 0 && childCount(indexPath) > 0) {
        retVal = true;
    }
    return retVal;
}
QString CustomDataModel::itemType(const QVariantList &indexPath) {
    QString retVal = QString::null;
    if (indexPath.length() == 1) {
        retVal = QString("item");
    }
    return retVal;
}
QVariant CustomDataModel::data(const QVariantList &indexPath) {
    QVariant retVal = QVariant();
    if (indexPath.length() == 1) {
        QVariantMap map = _internalDB.value(indexPath.value(0).toInt(NULL)).toMap();
        retVal = QVariant(map);
    } else if (indexPath.length() == 2) {
        //Only used to return attachment property of tasks
        QVariantMap map = _internalDB.value(indexPath.value(0).toInt(NULL)).toMap();
        QVariantMap attachments = map["attachments"].toList().value(indexPath.value(1).toInt(NULL)).toMap();
        retVal = QVariant(attachments);
    }
    return retVal;
}
/*
 * End of functions required by DataModel
 */

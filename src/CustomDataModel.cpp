#include "CustomDataModel.hpp"
#include <bb/data/JsonDataAccess>

using namespace bb::cascades;
using namespace bb::data;

CustomDataModel::CustomDataModel(QObject *parent, DataType dataType) : DataModel(parent) {
    _dataType = dataType;

    _netMan = NetworkManager::getInstance();
    _propMan = PropertiesManager::getInstance();
    _loginMan = LoginManager::getInstance();

    switch (_dataType) {
        case Task:
        case CompletedTask:
            getUrl = _netMan->getUrl.arg("tasks");
            addUrl = _netMan->addUrl.arg("tasks");
            editUrl = _netMan->editUrl.arg("tasks");
            removeUrl = _netMan->deleteUrl.arg("tasks");
            break;
        case Folder:
            getUrl = _netMan->getUrl.arg("folders");
            addUrl = _netMan->addUrl.arg("folders");
            editUrl = _netMan->editUrl.arg("folders");
            removeUrl = _netMan->deleteUrl.arg("folders");
            break;
        case Context:
            getUrl = _netMan->getUrl.arg("contexts");
            addUrl = _netMan->addUrl.arg("contexts");
            editUrl = _netMan->editUrl.arg("contexts");
            removeUrl = _netMan->deleteUrl.arg("contexts");
            break;
        case Goal:
            getUrl = _netMan->getUrl.arg("goals");
            addUrl = _netMan->addUrl.arg("goals");
            editUrl = _netMan->editUrl.arg("goals");
            removeUrl = _netMan->deleteUrl.arg("goals");
            break;
        case Location:
            getUrl = _netMan->getUrl.arg("locations");
            addUrl = _netMan->addUrl.arg("locations");
            editUrl = _netMan->editUrl.arg("locations");
            removeUrl = _netMan->deleteUrl.arg("locations");
            break;
        case AccountInfo:
            getUrl = _netMan->getUrl.arg("account");
            break;
        default:
            break;
    }

    bool ok;
    ok = connect(_netMan, SIGNAL(getReply(QString, QVariantList)),
            this, SLOT(onGetReply(QString, QVariantList)));
    Q_ASSERT(ok);
    ok = connect(_netMan, SIGNAL(addReply(QString, QVariantList)),
            this, SLOT(onAddReply(QString, QVariantList)));
    Q_ASSERT(ok);
    ok = connect(_netMan, SIGNAL(editReply(QString, QVariantList)),
            this, SLOT(onEditReply(QString, QVariantList)));
    Q_ASSERT(ok);
    ok = connect(_netMan, SIGNAL(removeReply(QString, QVariantList)),
            this, SLOT(onRemoveReply(QString, QVariantList)));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}
CustomDataModel::~CustomDataModel() {}

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

    url.setUrl(getUrl);
    if (_dataType == Task) {
        //incomplete tasks only
        urlData.addQueryItem("comp", QString::number(0));
        //fields id, title, modified, completed come automatically
        urlData.addEncodedQueryItem("fields", "duedate,note,folder,star,tag,priority,duetime,"
                "duedatemod,startdate,starttime,remind,repeat,status,length,context,goal,location");
    } else if (_dataType == CompletedTask) {
        //only get tasks modified in last n days, based on setting
        urlData.addQueryItem("after", QString::number(
                QDateTime::currentDateTimeUtc().toTime_t() - 86400 * _propMan->completedTaskAge()));
        //completed tasks only
        urlData.addQueryItem("comp", QString::number(1));
        //other fields not required for completed tasks view
        urlData.addQueryItem("fields", "note");
    }
    //Other datatypes don't require any additional arguments, they just get everything

    urlData.addQueryItem("access_token", _propMan->accessToken);
    qDebug() << Q_FUNC_INFO << url << urlData;
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    _netMan->sendRequest(req, urlData.encodedQuery());
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

bool compareLocations(QVariant &a, QVariant &b) {
    QVariantMap first = a.toMap();
    QVariantMap second = b.toMap();
    return first.value("id").toLongLong(NULL) <= second.value("id").toLongLong(NULL);
}

void CustomDataModel::sort() {
    if (_dataType == AccountInfo) {
        return;
    } else if (_dataType == Task) {
        qSort(_internalDB.begin(), _internalDB.end(), compareTasks);
    } else if (_dataType == CompletedTask) {
        qSort(_internalDB.begin(), _internalDB.end(), compareCompletedTasks);
    } else if (_dataType == Folder) {
        qSort(_internalDB.begin(), _internalDB.end(), compareFolders);
    } else if (_dataType == Context) {
        qSort(_internalDB.begin(), _internalDB.end(), compareContexts);
    } else if (_dataType == Goal) {
        qSort(_internalDB.begin(), _internalDB.end(), compareGoals);
    } else if (_dataType == Location) {
        qSort(_internalDB.begin(), _internalDB.end(), compareLocations);
    }
}

void CustomDataModel::add(QVariantMap data) {
    //Called by UI. Creates network request to add item.
    if (_dataType == AccountInfo) {
        return;
    }

    QUrl url;
    QUrl urlData;
    QVariantMap::iterator iter;

    url.setUrl(addUrl);
    if (_dataType == Task || _dataType == CompletedTask) {
        //Build task data string from user's input
        QString encodedData = QString("[{");
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
        urlData.addQueryItem("fields", "folder,tag,startdate,duedate,duedatemod,starttime,"
                "duetime,remind,repeat,status,star,priority,length,note,context,goal,location");
    } else {
        for (iter = data.begin(); iter != data.end(); ++iter) {
            urlData.addQueryItem(iter.key(), data[iter.key()].toString());
        }
    }

    urlData.addQueryItem("access_token", _propMan->accessToken);
    qDebug() << Q_FUNC_INFO << url;
    qDebug() << Q_FUNC_INFO << urlData;
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    _netMan->sendRequest(req, urlData.encodedQuery());
}

void CustomDataModel::edit(QVariantMap oldData, QVariantMap newData) {
    //Called by UI. Creates network request to edit item.
    if (_dataType == AccountInfo) {
        return;
    }

    if (oldData == newData) {
        //If nothing has changed, don't need to upload anything
        return;
    }

    QUrl url;
    QUrl urlData;
    QVariantMap::iterator iter;

    url.setUrl(editUrl);
    if (_dataType == Task || _dataType == CompletedTask) {
        QString encodedData = QString("[{");
        encodedData.append("\"id\":" + oldData["id"].toString());
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
        urlData.addQueryItem("fields", "folder,tag,startdate,duedate,duedatemod,starttime,"
                "duetime,remind,repeat,status,star,priority,length,note");
    } else {
        urlData.addQueryItem("id", newData["id"].toString());
        for (iter = newData.begin(); iter != newData.end(); ++iter) {
            if (oldData[iter.key()] != newData[iter.key()]) {
                urlData.addQueryItem(iter.key(), iter.value().toString());
            }
        }
    }

    urlData.addQueryItem("access_token", _propMan->accessToken);
    qDebug() << Q_FUNC_INFO << url;
    qDebug() << Q_FUNC_INFO << urlData;
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    _netMan->sendRequest(req, urlData.encodedQuery());
}

void CustomDataModel::remove(QVariantMap data) {
    //Called by UI. Creates network request to delete item.
    if (_dataType == AccountInfo) {
        return;
    }

    QUrl url;
    QUrl urlData;

    url.setUrl(removeUrl);
    if (_dataType == Task || _dataType == CompletedTask) {
        urlData.addQueryItem("tasks", "[\"" + data["id"].toString() + "\"]");
    } else {
        urlData.addQueryItem("id", data["id"].toString());
    }

    urlData.addQueryItem("access_token", _propMan->accessToken);
    qDebug() << Q_FUNC_INFO << url;
    qDebug() << Q_FUNC_INFO << urlData;
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    _netMan->sendRequest(req, urlData.encodedQuery());
}

/*
 * Slots
 */
void CustomDataModel::onGetReply(QString replyUrl, QVariantList dataList) {
    if (replyUrl != getUrl) {
        return;
    }

    foreach (QVariant v, dataList) {
        QVariantMap data = v.toMap();
        _internalDB.append(data);
        sort();
        qDebug() << Q_FUNC_INFO << "New data inserted into CustomDataModel:" << data;
    }
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
}

void CustomDataModel::onAddReply(QString replyUrl, QVariantList dataList) {
    if (_dataType == AccountInfo) {
        return;
    }
    if (replyUrl != addUrl) {
        return;
    }

    foreach (QVariant v, dataList) {
        QVariantMap data = v.toMap();
        _internalDB.append(data);
        sort();
        qDebug() << Q_FUNC_INFO << "New data added to CustomDataModel:" << data;
    }
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
}

void CustomDataModel::onEditReply(QString replyUrl, QVariantList dataList) {
    if (_dataType == AccountInfo) {
        return;
    }
    if (replyUrl != editUrl) {
        return;
    }

    foreach (QVariant v, dataList) {
        QVariantMap data = v.toMap();
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
        onAddReply(replyUrl, QVariantList() << data);
    }
}

void CustomDataModel::onRemoveReply(QString replyUrl, QVariantList dataList) {
    if (_dataType == AccountInfo) {
        return;
    }
    if (replyUrl != removeUrl) {
        return;
    }

    foreach (QVariant v, dataList) {
        QVariantMap data = v.toMap();
        qlonglong removeId = 0;

        if (_dataType == Task || _dataType == CompletedTask) {
            removeId = data.value("id").toLongLong(NULL);
        } else {
            removeId = data.value("deleted").toLongLong(NULL);
        }

        for (int i = 0; i < _internalDB.count(); ++i) {
            qlonglong currentId = _internalDB.value(i).toMap().value("id").toLongLong(NULL);
            if (currentId == removeId) {
                _internalDB.removeAt(i);
                sort();
                emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
                qDebug() << Q_FUNC_INFO << "Data removed from CustomDataModel:" << data;
                return;
            }
        }
    }
}

void CustomDataModel::onLogOut() {
    //Called by UI. Clear all stored info.
    _internalDB = QVariantList();
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
}
/*
 * End slots
 */

/*
 * Functions required by DataModel.
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

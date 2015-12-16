#include <bb/data/JsonDataAccess>
#include "FilterDataModel.hpp"

const QString FilterDataModel::getUrl = QString("http://api.toodledo.com/3/%1/get.php");
const QString FilterDataModel::addUrl = QString("http://api.toodledo.com/3/%1/add.php");
const QString FilterDataModel::editUrl = QString("http://api.toodledo.com/3/%1/edit.php");
const QString FilterDataModel::removeUrl = QString("http://api.toodledo.com/3/%1/delete.php");

using namespace bb::cascades;
using namespace bb::data;

FilterDataModel::FilterDataModel(QObject *parent) :
        GroupDataModel(parent), _netAccMan(new QNetworkAccessManager(this)), _filter(), _loginMan(
                LoginManager::getInstance()), _propMan(PropertiesManager::getInstance()), _allData(
                new QVariantList())
{
    setGrouping(ItemGrouping::ByFullValue);
    setSortingKeys(QStringList() << "sortingKey");

    bool ok;
    ok = connect(_netAccMan, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(onFinished(QNetworkReply*)));
    Q_ASSERT(ok);
    ok = connect(_loginMan, SIGNAL(networkStateChanged(bool)), this,
            SLOT(onNetworkStateChanged(bool)));
    Q_ASSERT(ok);
    ok = connect(_loginMan, SIGNAL(accessTokenRefreshed()), this, SLOT(onAccessTokenRefreshed()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

FilterDataModel::~FilterDataModel()
{
}

QVariant FilterDataModel::data(const QVariantList& indexPath)
{
    QVariant data = GroupDataModel::data(indexPath);

    // It's a header, so return custom header text ("Due in ...")
    if (itemsGrouped() && hasChildren(indexPath)) {
        if (data.toInt() == (uint) 9999999999) {
            return "No Due Date";
        }

        QDateTime datetime = QDateTime::fromTime_t(data.toInt());
        QString dateString = datetime.toString("dddd MMMM d yyyy");

        QString headerText = "Due " + dateString;
        return QVariant(headerText);
    }

    if (_filter.isEmpty()) {
        return data;
    }

    QVariantMap dataMap = data.toMap();

    // Need to verify that the data matches all our current filters
    bool matches = true;
    for (QVariantMap::const_iterator iter = _filter.begin(); iter != _filter.end(); ++iter) {
        QString value = dataMap[iter.key()].value<QString>();
        if (!value.contains(iter.value().toString(), Qt::CaseInsensitive)) {
            matches = false;
        }
    }

    if (matches) {
        return data;
    }

    return QVariant::Invalid;
}

void FilterDataModel::clearByType(QString type)
{
    QVariantMap item;
    // Remove all entries that have the type we're looking for
    for (int i = _allData->size() - 1; i >= 0; --i) {
        item = _allData->value(i).toMap();
        QString itemType = item["type"].value<QString>();
        if (itemType == type) {
            _allData->removeAt(i);
        }
    }

    emit emptyChanged(empty());
    emit itemsChanged(DataModelChangeType::AddRemove);
}

/*
 * Q_PROPERTY functions
 */
bool FilterDataModel::itemsGrouped()
{
    return grouping() == ItemGrouping::ByFullValue;
}

QVariantMap FilterDataModel::filter()
{
    return _filter;
}

bool FilterDataModel::empty()
{
    return size() == 0;
}

QVariantList FilterDataModel::allData()
{
    return *_allData;
}

void FilterDataModel::groupItems(bool group)
{
    if (group) {
        setGrouping(ItemGrouping::ByFullValue);
    } else {
        setGrouping(ItemGrouping::None);
    }
}

void FilterDataModel::setFilter(QVariantMap filter)
{
    _filter = filter;

    clear();

    QVariantMap item;
    bool matches = true;
    // Insert only the items from the full DM that match our criteria
    for (int i = 0; i < _allData->size(); ++i) {
        item = _allData->value(i).toMap();
        matches = true;

        for (QVariantMap::const_iterator iter = filter.begin(); iter != filter.end(); ++iter) {
            // Check each key in the filters, make sure the item in the DM has a matching value at that key
            QString value = item[iter.key()].value<QString>();

            if (!value.contains(iter.value().toString(), Qt::CaseInsensitive)) {
                matches = false;
                break;
            }
        }
        if (matches) {
            insert(item);
        }
    }

    emit emptyChanged(empty());
    emit itemsChanged(DataModelChangeType::AddRemove);
}

/*
 * Q_INVOKABLE functions
 */
void FilterDataModel::refresh(QString type)
{
    if (!_loginMan->isLoggedIn()) {
        qWarning() << Q_FUNC_INFO << "Can't update" << type
                << "because LoginManager indicated not logged in";
        return;
    }

    clearByType(type);

    get(type);
}

void FilterDataModel::get(QString type)
{
    // Retrieves all enties of the specified type from Toodledo
    QUrl url;
    QUrl data;

    url.setUrl(getUrl.arg(type));

    if ("tasks" == type) {
        // Incomplete tasks only
        data.addQueryItem("comp", QString::number(0));
        // id, title, modified, completed all come automatically
        data.addEncodedQueryItem("fields",
                "duedate,note,folder,star,tag,priority,duetime,duedatemod,startdate,starttime,remind,repeat,status,length,context,goal,location");
    }
    data.addQueryItem("access_token", _propMan->accessToken);

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setAttribute(QNetworkRequest::User, QString(type));

    //Tells UI to show activity indicator
    emit networkRequestStarted();
    qDebug() << Q_FUNC_INFO << "Sending" << data.encodedQuery() << "to" << req.url().toString();
    _netAccMan->post(req, data.encodedQuery());
}

void FilterDataModel::addItem(QString type, QVariantMap data)
{
    // Called by UI; creates network request to add a new item of specified type to data model
    // Doesn't actually get added until the reply comes back
    QUrl url;
    QUrl urlData;
    QVariantMap::iterator iter;

    url.setUrl(addUrl.arg(type));

    QString encodedData = QString("[{");
    for (iter = data.begin(); iter != data.end(); ++iter) {
        encodedData.append(",\"" + iter.key() + "\":");
        bool ok;
        int value = data[iter.key()].toInt(&ok);
        Q_UNUSED(value);
        if (ok) {
            // numeric values
            encodedData.append(iter.value().toString());
        } else {
            // String values (need quotes around value)
            encodedData.append("\"" + iter.value().toString().replace("\"", "'") + "\"");
        }
    }
    encodedData.append("}]");
    encodedData.remove(2, 1); // Remove initial comma YES THIS IS MESSY I KNOW STOP YELLING.
    encodedData = encodedData.replace("\n", "\\n").replace(" ", "+"); // Sanitization required by API
    encodedData = QUrl::toPercentEncoding(encodedData, "\"{}[]+\\,:", ""); // Sanitization required by API

    urlData.addEncodedQueryItem("tasks", encodedData.toAscii());
    urlData.addQueryItem("fields", "folder,tag,startdate,duedate,duedatemod,starttime,"
            "duetime,remind,repeat,status,star,priority,length,note,context,goal,location");
    urlData.addQueryItem("access_token", _propMan->accessToken);

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setAttribute(QNetworkRequest::User, QString(type));

    //Tells UI to show activity indicator
    emit networkRequestStarted();
    qDebug() << Q_FUNC_INFO << "Sending" << urlData.encodedQuery() << "to" << req.url().toString();
    _netAccMan->post(req, urlData.encodedQuery());
}

void FilterDataModel::editItem(QString type, QVariantMap data)
{
    // Called by UI; creates network request to edit an item in the data model of the specified type
    // Doesn't actually get updated in the data model until the reply comes back

    // In the event that save was hit, but no values actually changed
    // (the 1 present key will be the id, which is always set anyway)
    if (data.keys().length() <= 1) {
        return;
    }

    QUrl url;
    QUrl urlData;
    QVariantMap::iterator iter;

    url.setUrl(editUrl.arg(type));

    QString encodedData = QString("[{");
    for (iter = data.begin(); iter != data.end(); ++iter) {
        encodedData.append(",\"" + iter.key() + "\":");
        bool ok;
        int value = data[iter.key()].toInt(&ok);
        Q_UNUSED(value);
        if (ok) {
            // numeric values
            encodedData.append(iter.value().toString());
        } else {
            // String values (need quotes around value)
            encodedData.append("\"" + iter.value().toString().replace("\"", "'") + "\"");
        }
    }
    encodedData.append("}]");
    encodedData.remove(2, 1); // Remove initial comma YES THIS IS MESSY I KNOW STOP YELLING.
    encodedData = encodedData.replace("\n", "\\n").replace(" ", "+"); // Sanitization required by API
    encodedData = QUrl::toPercentEncoding(encodedData, "\"{}[]+\\,:", ""); // Sanitization required by API

    urlData.addEncodedQueryItem("tasks", encodedData.toAscii());
    urlData.addQueryItem("fields", "folder,tag,startdate,duedate,duedatemod,starttime,"
            "duetime,remind,repeat,status,star,priority,length,note,context,goal,location");
    urlData.addQueryItem("access_token", _propMan->accessToken);

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setAttribute(QNetworkRequest::User, QString(type));

    //Tells UI to show activity indicator
    emit networkRequestStarted();
    qDebug() << Q_FUNC_INFO << "Sending" << urlData.encodedQuery() << "to" << req.url().toString();
    _netAccMan->post(req, urlData.encodedQuery());
}

void FilterDataModel::removeItem(QString type, QVariantMap data)
{
    // Called by UI; creates network request to remove an item of the specified type from the data model
    // Doesn't actually get removed from the data model until the reply comes back
    QUrl url;
    QUrl urlData;

    url.setUrl(removeUrl.arg(type));
    // This is only used for adding tasks... could just not have the type arg, but meh
    urlData.addQueryItem("tasks", "[\"" + data["id"].toString() + "\"]");
    urlData.addQueryItem("access_token", _propMan->accessToken);

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setAttribute(QNetworkRequest::User, QString(type));

    //Tells UI to show activity indicator
    emit networkRequestStarted();
    qDebug() << Q_FUNC_INFO << "Sending" << urlData.encodedQuery() << "to" << req.url().toString();
    _netAccMan->post(req, urlData.encodedQuery());
}

/*
 * Slots
 */
void FilterDataModel::onFinished(QNetworkReply *reply)
{
    QString response = reply->readAll();
    QString replyDataType = reply->request().attribute(QNetworkRequest::User).toString();
    qDebug() << Q_FUNC_INFO << "Received" << replyDataType << response;

    JsonDataAccess jda;
    QVariantList dataList = jda.loadFromBuffer(response).value<QVariantList>();
    if (jda.hasError()) {
        qWarning() << Q_FUNC_INFO << "Error reading network response into list:" << jda.error();
        reply->deleteLater();
        // Tells UI to hide activity indicator
        emit networkRequestFinished();
        return;
    }

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << Q_FUNC_INFO << "Reply from" << reply->url() << "contains error"
                << reply->errorString();
        reply->deleteLater();
        // Tells UI to hide activity indicator
        emit networkRequestFinished();
        return;
    }

    QString replyUrl = reply->url().toString(QUrl::RemoveQuery);

    if (replyDataType == "account") {
        QVariantMap oldInfo = _propMan->accountInfo;

        // Account info comes as a map, not a list of maps
        QVariantMap newInfo = jda.loadFromBuffer(response).value<QVariantMap>();
        if (jda.hasError()) {
            qWarning() << Q_FUNC_INFO << "Error reading network response into map:" << jda.error();
            reply->deleteLater();
            // Tells UI to hide activity indicator
            emit networkRequestFinished();
            return;
        }

        // We've updated the account info; now check and see what needs to be updated
        if (oldInfo.value("lastedit_task").toInt(0) < newInfo.value("lastedit_task").toInt(0)) {
            refresh("tasks");
        }
        if (oldInfo.value("lastedit_folder").toInt(0) < newInfo.value("lastedit_folder").toInt(0)) {
            refresh("folders");
        }
        if (oldInfo.value("lastedit_context").toInt(0)
                < newInfo.value("lastedit_context").toInt(0)) {
            refresh("contexts");
        }
        if (oldInfo.value("lastedit_goal").toInt(0) < newInfo.value("lastedit_goal").toInt(0)) {
            refresh("goals");
        }
        if (oldInfo.value("lastedit_location").toInt(0)
                < newInfo.value("lastedit_location").toInt(0)) {
            refresh("locations");
        }

        _propMan->accountInfo = newInfo;
    } else if (replyDataType == "tasks") {
        if (replyUrl.contains("get") || replyUrl.contains("add")) {
            if (replyUrl.contains("get") && dataList.length() > 0) {
                // Discard summary item (only comes with getting tasks)
                dataList.pop_front();
            }
            QVariantMap data;
            for (int i = 0; i < dataList.size(); ++i) {
                data = dataList.value(i).toMap();
                data["type"] = replyDataType;

                // This is ridiculous and weird, but it's an easy way to
                // a) make tasks without due dates go to the bottom of the list, and
                // b) group tasks by the day they're due
                int dueDate = data["duedate"].toInt(0);
                data["sortingKey"] = dueDate - (dueDate % 86400);
                data["sortingKey"] = dueDate > 0 ? dueDate : 9999999999;

                _allData->append(data);
                qDebug() << Q_FUNC_INFO << "Inserted a" << data["type"].toString() << "called"
                        << data["title"].toString();
            }
        } else {
            // Edit and delete are similar - only difference is what we do with the item in the DM that has an ID
            // that matches the ID in the reply (replace it vs remove it)
            QVariantMap data;
            QVariantMap compareTo;
            for (int i = 0; i < dataList.size(); ++i) {
                data = dataList.value(i).toMap();
                data["type"] = replyDataType;

                for (int j = 0; j < _allData->size(); ++j) {
                    compareTo = _allData->value(j).toMap();
                    if (data["type"] == replyDataType && data["id"] == compareTo["id"]) {
                        if (replyUrl.contains("edit")) {
                            _allData->replace(j, data);
                            qDebug() << Q_FUNC_INFO << "Edited a" << data["type"].toString()
                                    << "called" << data["title"].toString();
                        } else {
                            _allData->removeAt(j);
                            qDebug() << Q_FUNC_INFO << "Removed a" << data["type"].toString()
                                    << "called" << data["title"].toString();
                        }
                        break;
                    }
                }
            }
        }
    } else {
        // Actions for FCGL are all the same - and it's only get anyway
        QVariantMap data;
        for (int i = 0; i < dataList.size(); ++i) {
            data = dataList.value(i).toMap();
            data["type"] = replyDataType;
            data["title"] = data["name"].toString();
            _allData->append(data);
            qDebug() << Q_FUNC_INFO << "Inserted a" << data["type"].toString() << "called"
                    << data["title"].toString();
        }
    }

    reply->deleteLater();
    // Tells UI to hide activity indicator
    emit networkRequestFinished();
    emit emptyChanged(empty());
    emit itemsChanged(DataModelChangeType::AddRemove);
}

void FilterDataModel::onLogOut()
{
    clear();
    emit emptyChanged(empty());
    emit itemsChanged(DataModelChangeType::AddRemove);
}

void FilterDataModel::onNetworkStateChanged(bool online)
{
    if (online) {
        refresh("account");
    }
}

void FilterDataModel::onAccessTokenRefreshed()
{
    refresh("account");
}

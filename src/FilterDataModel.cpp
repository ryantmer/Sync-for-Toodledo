#include <bb/data/JsonDataAccess>
#include "FilterDataModel.hpp"

const QString FilterDataModel::getUrl = QString("http://api.toodledo.com/3/%1/get.php");
const QString FilterDataModel::addUrl = QString("http://api.toodledo.com/3/%1/add.php");
const QString FilterDataModel::editUrl = QString("http://api.toodledo.com/3/%1/edit.php");
const QString FilterDataModel::deleteUrl = QString("http://api.toodledo.com/3/%1/delete.php");

using namespace bb::cascades;
using namespace bb::data;

FilterDataModel::FilterDataModel(QObject *parent) :
        GroupDataModel(parent), _netAccMan(new QNetworkAccessManager(this)), _filter(""), _filterOn(
                "type"), _loginMan(LoginManager::getInstance()), _propMan(
                PropertiesManager::getInstance()), _fullDM(new QMapListDataModel())
{
    bool ok;
    ok = connect(_netAccMan, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(onFinished(QNetworkReply*)));
    Q_ASSERT(ok);
    ok = connect(_loginMan, SIGNAL(networkStateChanged(bool)), this,
            SLOT(onNetworkStateChanged(bool)));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

FilterDataModel::~FilterDataModel()
{
}

QVariant FilterDataModel::data(const QVariantList& indexPath)
{
    QVariant data = GroupDataModel::data(indexPath);

    if (_filter == NULL || _filter == "") {
        return data;
    }

    QVariantMap dataMap = data.toMap();
    QString filterOnValue = dataMap[_filterOn].value<QString>();

    if (filterOnValue.contains(_filter, Qt::CaseInsensitive)) {
        return data;
    }

    return QVariant::Invalid;
}

void FilterDataModel::clearByType(QString type)
{
    QVariantMap item;
    // Remove all entries that have the type we're looking for
    for (int i = _fullDM->size() - 1; i >= 0; --i) {
        item = _fullDM->value(i);
        QString itemType = item["type"].value<QString>();
        qWarning() << "ITER" << i << itemType;
        if (itemType == type) {
            _fullDM->removeAt(i);
        }
    }
    emit emptyChanged(empty());
}

/*
 * Q_PROPERTY functions
 */
QString FilterDataModel::filter()
{
    return _filter;
}

void FilterDataModel::setFilter(QString filter)
{
    _filter = filter;

    clear();

    QVariantMap item;
    // Insert only the items from the full DM that match our criteria
    for (int i = 0; i < _fullDM->size(); ++i) {
        item = _fullDM->value(i);
        QString filterOnValue = item[_filterOn].value<QString>();
        if (filterOnValue.contains(_filter, Qt::CaseInsensitive)) {
            insert(item);
        }
    }

    emit emptyChanged(empty());
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
}

QString FilterDataModel::filterOn()
{
    return _filterOn;
}

void FilterDataModel::setFilterOn(QString filterOn)
{
    _filterOn = filterOn;
}

bool FilterDataModel::isFiltered(const QVariantList& indexPath)
{
    if (_filter == NULL || _filter == "") {
        return true;
    }

    QVariant data = _fullDM->data(indexPath);
    QVariantMap dataMap = data.toMap();

    QString str = dataMap[_filterOn].value<QString>();
    return str.contains(_filter, Qt::CaseInsensitive);
}

bool FilterDataModel::empty()
{
    return size() == 0;
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
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        QString replyUrl = reply->url().toString(QUrl::RemoveQuery);

        if (replyDataType == "account") {
            // We've updated the account info; now check and see what needs to be updated

            QVariantMap oldInfo = _propMan->accountInfo;

            // Account info comes as a map, not a list of maps
            QVariantMap newInfo = jda.loadFromBuffer(response).value<QVariantMap>();
            if (jda.hasError()) {
                qWarning() << Q_FUNC_INFO << "Error reading network response into map:" << jda.error();
                return;
            }

            refresh("tasks");
            refresh("folders");
            refresh("contexts");
            refresh("goals");
            refresh("locations");
        } else if (replyDataType == "tasks") {
            if (dataList.length() > 0) {
                // Discard summary item (only comes with tasks)
                dataList.pop_front();
            }

            if (replyUrl.contains("get")) {
                QVariantMap data;
                for (int i = 0; i < dataList.size(); ++i) {
                    data = dataList.value(i).toMap();
                    data["type"] = replyDataType;
                    _fullDM->append(data);
                    qDebug() << Q_FUNC_INFO << "Inserted a" << data["type"].toString() << "called" << data["title"].toString();
                }
            }
        } else {
            if (replyUrl.contains("get")) {
                QVariantMap data;
                for (int i = 0; i < dataList.size(); ++i) {
                    data = dataList.value(i).toMap();
                    data["type"] = replyDataType;
                    data["title"] = data["name"].toString();
                    _fullDM->append(data);
                    qDebug() << Q_FUNC_INFO << "Inserted a" << data["type"].toString() << "called" << data["title"].toString();
                }
            }
        }
    } else {
        qWarning() << Q_FUNC_INFO << "Reply from" << reply->url() << "contains error" << reply->errorString();
    }

    reply->deleteLater();
    // Tells UI to hide activity indicator
    emit networkRequestFinished();
    emit emptyChanged(empty());
}

void FilterDataModel::onLogOut()
{
    clear();
    emit emptyChanged(empty());
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
}

void FilterDataModel::onNetworkStateChanged(bool online)
{
    if (online) {
        refresh("account");
    }
}

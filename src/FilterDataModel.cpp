#include <bb/data/JsonDataAccess>
#include "FilterDataModel.hpp"

const QString FilterDataModel::getUrl = QString("http://api.toodledo.com/3/%1s/get.php");
const QString FilterDataModel::addUrl = QString("http://api.toodledo.com/3/%1s/add.php");
const QString FilterDataModel::editUrl = QString("http://api.toodledo.com/3/%1s/edit.php");
const QString FilterDataModel::deleteUrl = QString("http://api.toodledo.com/3/%1s/delete.php");

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
    // Then filter out the ones we don't want
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
void FilterDataModel::refresh()
{
    if (!_loginMan->isLoggedIn()) {
        qWarning() << Q_FUNC_INFO << "LoginManager indicated not logged in";
        return;
    }

    clear();

    get("task");
    get("folder");
    get("context");
    get("goal");
    get("location");
}

void FilterDataModel::get(QString type)
{
    QUrl url;
    QUrl data;

    url.setUrl(getUrl.arg(type));

    if ("task" == type) {
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
        qWarning() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        QString replyUrl = reply->url().toString(QUrl::RemoveQuery);

        if (replyUrl.contains("get")) {
            if (dataList.length() > 0 && replyDataType == "task") {
                // Discard summary item (only comes with tasks)
                dataList.pop_front();
            }

            QVariantMap data;
            for (int i = 0; i < dataList.size(); ++i) {
                data = dataList.value(i).toMap();

                data["type"] = replyDataType;
                // Some use title (maybe just task?), some use name
                if (data["title"].isNull()) {
                    data["title"] = data["name"].toString();
                }

                _fullDM->append(data);
                qDebug() << Q_FUNC_INFO << "Inserted a" << data["type"].toString() << "called"
                        << data["title"].toString();
            }
        }
    } else {
        qWarning() << Q_FUNC_INFO << "Reply from" << reply->url() << "contains error"
                << reply->errorString();
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

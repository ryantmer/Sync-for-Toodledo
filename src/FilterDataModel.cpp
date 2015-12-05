#include "FilterDataModel.hpp"

const char * FilterDataModel::DataTypeStrings[9] = {
    "UndefinedType",
    "Task",
    "Hotlist",
    "CompletedTask",
    "Folder",
    "Context",
    "Goal",
    "Location",
    "AccountInfo"
};

using namespace bb::cascades;

FilterDataModel::FilterDataModel(QObject *parent) :
        GroupDataModel(parent), _filter(""), _filterOn(""), _firstRun(true),
        _netMan(NetworkManager::getInstance()), _loginMan(LoginManager::getInstance()),
                _propMan(PropertiesManager::getInstance())
{
    bool ok;
    ok = connect(_netMan, SIGNAL(addReply(int, QVariantList)), this,
            SLOT(onRemoveReply(int, QVariantList)));
    Q_ASSERT(ok);
    ok = connect(_netMan, SIGNAL(editReply(int, QVariantList)), this,
            SLOT(onEditReply(int, QVariantList)));
    Q_ASSERT(ok);
    ok = connect(_netMan, SIGNAL(getReply(int, QVariantList)), this,
            SLOT(onGetReply(int, QVariantList)));
    Q_ASSERT(ok);
    ok = connect(_netMan, SIGNAL(removeReply(int, QVariantList)), this,
            SLOT(onRemoveReply(int, QVariantList)));
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
    // Keep the full set of unfiltered data
    if (_firstRun) {
        QList<QVariantMap> items = toListOfMaps();
        _originalItems = items;
        _firstRun = false;
    }

    // In any case, re-insert all the items into the data model
    clear();
    insertList(_originalItems);

    _filter = filter;

    QVariantMap item;
    QList<QVariantMap> filteredItems;

    clear();

    // Then filter out the ones we don't want
    foreach(item, _originalItems){
    QString filterOnValue = item[_filterOn].value<QString>();
    if(filterOnValue.contains(_filter, Qt::CaseInsensitive)) {
        insert(item);
    }
}
emit
    itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
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

/*
 * Q_INVOKABLE functions
 */
void FilterDataModel::refresh()
{
    if (!_netMan->isConnected()) {
        qWarning() << Q_FUNC_INFO << "NetworkManager indicated no network connection";
        return;
    }
    if (!_loginMan->isLoggedIn()) {
        qWarning() << Q_FUNC_INFO << "LoginManager indicated not logged in";
    }

    clear();

    get("tasks");
    get("folders");
    get("contexts");
    get("goals");
    get("locations");
}

void FilterDataModel::get(QString type) {
    QUrl url;
    QUrl urlData;

    url.setUrl(_netMan->getUrl.arg(type));

    if ("tasks" == type) {
        // Incomplete tasks only
        urlData.addQueryItem("comp", QString::number(0));
        // id, title, modified, completed all come automatically
        urlData.addEncodedQueryItem("fields", "duedate,note,folder,star,tag,priority,duetime,duedatemod,startdate,starttime,remind,repeat,status,length,context,goal,location");
    }

    urlData.addQueryItem("access_token", _propMan->accessToken);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setAttribute(QNetworkRequest::User, QVariant(type));
    _netMan->sendRequest(req, urlData.encodedQuery());
}

/*
 * Slots
 */
void FilterDataModel::onAddReply(int replyDataType, QVariantList dataList)
{
    qDebug() << Q_FUNC_INFO << "Received" << DataTypeStrings[replyDataType] << "data";

    foreach (QVariant v, dataList) {
        QVariantMap data = v.toMap();

        data["type"] = DataTypeStrings[replyDataType];
        if (data["title"].isNull()) {
            data["title"] = data["name"].toString();
        }

        insert(data);
        qDebug() << Q_FUNC_INFO << "Added a" << data["type"].toString() << "called" << data["title"].toString() << "into everything:" << data;
    }
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
}
void FilterDataModel::onEditReply(int replyDataType, QVariantList dataList)
{
    qDebug() << Q_FUNC_INFO << "Received" << DataTypeStrings[replyDataType] << "data";

    foreach (QVariant v, dataList) {
        QVariantMap data = v.toMap();

        data["type"] = DataTypeStrings[replyDataType];
        if (data["title"].isNull()) {
            data["title"] = data["name"].toString();
        }

        insert(data);
        qDebug() << Q_FUNC_INFO << "Edited a" << data["type"].toString() << "called" << data["title"].toString() << "into everything:" << data;
    }
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
}
void FilterDataModel::onGetReply(int replyDataType, QVariantList dataList)
{
    qDebug() << Q_FUNC_INFO << "Received" << DataTypeStrings[replyDataType] << "data";

    foreach (QVariant v, dataList) {
        QVariantMap data = v.toMap();

        data["type"] = DataTypeStrings[replyDataType];
        if (data["title"].isNull()) {
            data["title"] = data["name"].toString();
        }

        insert(data);
        qDebug() << Q_FUNC_INFO << "Got a" << data["type"].toString() << "called" << data["title"].toString() << "into everything:" << data;
    }
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
}

void FilterDataModel::onRemoveReply(int replyDataType, QVariantList dataList)
{
    qDebug() << Q_FUNC_INFO << "Received" << DataTypeStrings[replyDataType] << "data";

    foreach (QVariant v, dataList) {
        QVariantMap data = v.toMap();
        remove(data);
        qDebug() << Q_FUNC_INFO << "Removed a" << data["type"].toString() << "called" << data["title"].toString() << "into everything:" << data;
    }
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
}

void FilterDataModel::onLogOut()
{
    clear();
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
}

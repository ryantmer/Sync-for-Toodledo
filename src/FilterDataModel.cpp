#include "FilterDataModel.hpp"
#include "CustomDataModel.hpp"

using namespace bb::cascades;

FilterDataModel::FilterDataModel(QObject *parent) :
        GroupDataModel(parent), _filter(""), _filterOn(""), _firstRun(true), _netMan(
                NetworkManager::getInstance())
{
    bool ok = connect(_netMan, SIGNAL(getReply(int, QVariantList)), this,
            SLOT(onGetReply(int, QVariantList)));
    Q_ASSERT(ok);
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
    QString str = dataMap[_filterOn].value<QString>();

    if (str.contains(_filter, Qt::CaseInsensitive)) {
        return data;
    }

    return QVariant::Invalid;
}

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
    foreach(item, _originalItems) {
        QString filterOnValue = item[_filterOn].value<QString>();
        if(filterOnValue.contains(_filter, Qt::CaseInsensitive)) {
            insert(item);
        }
    }
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

void FilterDataModel::onGetReply(int replyDataType, QVariantList dataList)
{
    qDebug() << Q_FUNC_INFO << "Neat: received get reply";

    foreach (QVariant v, dataList) {
        QVariantMap data = v.toMap();

        data["type"] = CustomDataModel::DataTypeStrings[replyDataType];
        if (data["title"].isNull()) {
            data["title"] = data["name"].toString();
        }

        insert(data);
        qDebug() << Q_FUNC_INFO << "Inserted a" << data["type"].toString() << "called" << data["title"].toString() << "into everything:" << data;
    }
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
}

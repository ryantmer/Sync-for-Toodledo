#include "CustomDataModel.hpp"
#include <bb/data/JsonDataAccess>

using namespace bb::cascades;
using namespace bb::data;

CustomDataModel::CustomDataModel(QObject *parent) : DataModel(parent) {
    _dataType = UndefinedType;
}

CustomDataModel::~CustomDataModel() {}

void CustomDataModel::setDataType(DataType dataType) {
    _dataType = dataType;
}

void CustomDataModel::clear() {
    _internalDB.clear();
}

QVariant CustomDataModel::firstEntry() {
    if (_internalDB.length() > 0) {
        return _internalDB.first();
    } else {
        return QVariant();
    }
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

    return first.value("ord").toLongLong(NULL) < second.value("ord").toLongLong(NULL);
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
        case Context:
        case Goal:
        case Location:
        default:
            break;
    }
}

/*
 * Slots
 */
void CustomDataModel::onAdd(QVariantMap data) {
    _internalDB.append(data);
    sort();
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
    qDebug() << Q_FUNC_INFO << "New data added to CustomDataModel:" << data;
}

void CustomDataModel::onEdit(QVariantMap data) {
    qlonglong removeId = data.value("id").toLongLong(NULL);
    for (int i = 0; i < _internalDB.count(); ++i) {
        qlonglong currentId = _internalDB.value(i).toMap().value("id").toLongLong(NULL);
        if (currentId == removeId) {
            //Create a new QVM, overwriting pre-existing values, and inserting new ones if required
            QVariantMap amalgamatedData = _internalDB.value(i).toMap();
            for (QVariantMap::const_iterator iter = data.begin(); iter != data.end(); ++iter) {
                amalgamatedData.insert(iter.key(), iter.value());
            }
            _internalDB.replace(i, amalgamatedData);

            if (_dataType == Task) {
                //Special case for a task that is completed; remove from CDM
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
    onAdd(data);
}

void CustomDataModel::onRemove(QVariantMap data) {
    qlonglong removeId;
    switch (_dataType) {
        case Task:
            removeId = data.value("id").toLongLong(NULL);
            break;
        case Folder:
            removeId = data.value("deleted").toLongLong(NULL);
            break;
        case CompletedTask:
        case Context:
        case Goal:
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
    if (indexPath.length() == 0) {
        return _internalDB.length();
    } else if (indexPath.length() == 1) {
        //Only a task's attachment property has children
        QVariantMap map = _internalDB.value(indexPath.value(0).toInt(NULL)).toMap();
        if (map.contains("attachment")) {
            int count = map["attachment"].toList().length();
            return count;
        }
    }
    return 0;
}
bool CustomDataModel::hasChildren(const QVariantList &indexPath) {
    if (indexPath.length() == 0) {
        return true;
    } else if (indexPath.length() == 1 && itemType(indexPath) == QString("task")) {
        return true;
    }
    return false;
}
QString CustomDataModel::itemType(const QVariantList &indexPath) {
    if (indexPath.length() == 1) {
        return QString("item");
    }
    return QString::null;
}
QVariant CustomDataModel::data(const QVariantList &indexPath) {
    if (indexPath.length() == 1) {
        QVariantMap map = _internalDB.value(indexPath.value(0).toInt(NULL)).toMap();
        return QVariant(map);
    } else if (indexPath.length() == 2) {
        //Only used to return attachment property of tasks
        QVariantMap map = _internalDB.value(indexPath.value(0).toInt(NULL)).toMap();
        QVariantMap attachments = map["attachments"].toList().value(indexPath.value(1).toInt(NULL)).toMap();
        return QVariant(attachments);
    }
    return QVariant();
}
/*
 * End of functions required by DataModel
 */

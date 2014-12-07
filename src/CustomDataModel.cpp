#include "CustomDataModel.hpp"
#include <bb/data/JsonDataAccess>

using namespace bb::cascades;
using namespace bb::data;

CustomDataModel::CustomDataModel(QObject *parent) : DataModel(parent) {}

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

bool compareTasksByDueDate(QVariant &a, QVariant &b) {
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

bool compareFoldersByOrd(QVariant &a, QVariant &b) {
    QVariantMap first = a.toMap();
    QVariantMap second = b.toMap();

    return first.value("ord").toLongLong(NULL) < second.value("ord").toLongLong(NULL);
}

void CustomDataModel::sort() {
    if (_dataType == Folder) {
        qSort(_internalDB.begin(), _internalDB.end(), compareFoldersByOrd);
    } else if (_dataType == Task) {
        qSort(_internalDB.begin(), _internalDB.end(), compareTasksByDueDate);
    }
}

/*
 * Slots
 */
void CustomDataModel::onTaskEdited(QVariantMap task) {
    //Find task in datamodel, if it exists
    for (int i = 0; i < _internalDB.count(); ++i) {
        if (_internalDB.value(i).toMap().value("id").toLongLong(NULL) == task.value("id").toLongLong(NULL)) {
            //Replace values in local task with new values, by key
            QVariantMap localTask = _internalDB.value(i).toMap();
            for (QVariantMap::const_iterator iter = task.begin(); iter != task.end(); ++iter) {
                localTask.insert(iter.key(), iter.value());
            }
            _internalDB.replace(i, localTask);

            if (_internalDB.value(i).toMap().value("completed").toLongLong(NULL) == 0) {
                sort();
//                emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
                qDebug() << Q_FUNC_INFO << "Task edited in CustomDataModel:" << task;
            } else {
                _internalDB.removeAt(i);
                sort();
                emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
                qDebug() << Q_FUNC_INFO << "Task removed from CustomDataModel:" << task;
            }
            return;
        }
    }

    //If not found in datamodel, then add as a new task
    onTaskAdded(task);
}

void CustomDataModel::onTaskAdded(QVariantMap task) {
    //Just add task to end of list and re-sort list
    _internalDB.append(task);

    sort();
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
    qDebug() << Q_FUNC_INFO << "New task added to CustomDataModel:" << task;
}

void CustomDataModel::onTaskRemoved(QVariantMap task) {
    for (int i = 0; i < _internalDB.count(); ++i) {
        if (_internalDB.value(i).toMap().value("id").toLongLong(NULL) == task.value("id").toLongLong(NULL)) {
            _internalDB.removeAt(i);
            sort();
            emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
            qDebug() << Q_FUNC_INFO << "Task removed from CustomDataModel:" << task;
            return;
        }
    }
}

void CustomDataModel::onFolderEdited(QVariantMap folder) {
    //Find folder in datamodel, if it exists
    for (int i = 0; i < _internalDB.count(); ++i) {
        if (_internalDB.value(i).toMap().value("id").toLongLong(NULL) == folder.value("id").toLongLong(NULL)) {
            //Replace values in local folder with new values, by key
            QVariantMap localFolder = _internalDB.value(i).toMap();
            for (QVariantMap::const_iterator iter = folder.begin(); iter != folder.end(); ++iter) {
                localFolder.insert(iter.key(), iter.value());
            }
            _internalDB.replace(i, localFolder);

            emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
            qDebug() << Q_FUNC_INFO << "Folder edited in CustomDataModel:" << folder;
            return;
        }
    }

    //If not found in datamodel, then add as a new folder
    onFolderAdded(folder);
}

void CustomDataModel::onFolderAdded(QVariantMap folder) {
    if (folder.value("archived").toInt(NULL) == 0) {
        _internalDB.append(folder);
        emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
        qDebug() << Q_FUNC_INFO << "New folder added to CustomDataModel:" << folder;
    } else {
        //Don't add archived folders
        return;
    }
}

void CustomDataModel::onFolderRemoved(QVariantMap folder) {
    for (int i = 0; i < _internalDB.count(); ++i) {
        if (_internalDB.value(i).toMap().value("id").toLongLong(NULL) == folder.value("deleted").toLongLong(NULL)) {
            _internalDB.removeAt(i);
            emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
            qDebug() << Q_FUNC_INFO << "Folder removed from CustomDataModel:" << folder;
            return;
        }
    }
}

void CustomDataModel::onLoggedOut() {
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

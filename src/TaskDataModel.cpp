#include "TaskDataModel.hpp"
#include <bb/data/JsonDataAccess>

using namespace bb::cascades;
using namespace bb::data;

TaskDataModel::TaskDataModel(QObject *parent) : DataModel(parent) {}

TaskDataModel::~TaskDataModel() {}

QVariant TaskDataModel::firstEntry() {
    if (taskDB.length() > 0) {
        return taskDB.first();
    } else {
        return QVariant();
    }
}

void TaskDataModel::clear() {
    taskDB.clear();
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

void TaskDataModel::sortTasksByDueDate() {
    qSort(taskDB.begin(), taskDB.end(), compareTasksByDueDate);
}

/*
 * Slots
 */
void TaskDataModel::onTaskEdited(QVariantMap task) {
    //Find task in datamodel, if it exists
    for (int i = 0; i < taskDB.count(); ++i) {
        if (taskDB.value(i).toMap().value("id").toLongLong(NULL) == task.value("id").toLongLong(NULL)) {
            //Replace values in local task with new values, by key
            QVariantMap localTask = taskDB.value(i).toMap();
            for (QVariantMap::const_iterator iter = task.begin(); iter != task.end(); ++iter) {
                localTask.insert(iter.key(), iter.value());
            }
            taskDB.replace(i, localTask);

            if (taskDB.value(i).toMap().value("completed").toLongLong(NULL) == 0) {
                sortTasksByDueDate();
//                emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
                qDebug() << Q_FUNC_INFO << "Task edited in TaskDataModel:" << task;
            } else {
                taskDB.removeAt(i);
                sortTasksByDueDate();
                emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
                qDebug() << Q_FUNC_INFO << "Task removed from TaskDataModel:" << task;
            }
            return;
        }
    }

    //If not found in datamodel, then add as a new task
    onTaskAdded(task);
}

void TaskDataModel::onTaskAdded(QVariantMap task) {
    //Just add task to end of list and re-sort list
    taskDB.append(task);

    sortTasksByDueDate();
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
    qDebug() << Q_FUNC_INFO << "New task added to TaskDataModel:" << task;
}

void TaskDataModel::onTaskRemoved(QVariantMap task) {
    for (int i = 0; i < taskDB.count(); ++i) {
        if (taskDB.value(i).toMap().value("id").toLongLong(NULL) == task.value("id").toLongLong(NULL)) {
            taskDB.removeAt(i);
            sortTasksByDueDate();
            emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
            qDebug() << Q_FUNC_INFO << "Task removed from TaskDataModel:" << task;
            return;
        }
    }
}

void TaskDataModel::onLoggedOut() {
    //Clear all stored tasks
    taskDB = QVariantList();
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
}
/*
 * End slots
 */

/*
 * Functions required by DataModel. Not sure if these work.
 */
int TaskDataModel::childCount(const QVariantList &indexPath) {
    if (indexPath.length() == 0) {
        return taskDB.length();
    } else if (indexPath.length() == 1) {
        //Only a task's attachment property has children
        QVariantMap map = taskDB.value(indexPath.value(0).toInt(NULL)).toMap();
        if (map.contains("attachment")) {
            int count = map["attachment"].toList().length();
            return count;
        }
    }
    return 0;
}
bool TaskDataModel::hasChildren(const QVariantList &indexPath) {
    if (indexPath.length() == 0) {
        return true;
    } else if (indexPath.length() == 1 && itemType(indexPath) == QString("task")) {
        return true;
    }
    return false;
}
QString TaskDataModel::itemType(const QVariantList &indexPath) {
    if (indexPath.length() == 1) {
        return QString("task");
    }
    return QString::null;
}
QVariant TaskDataModel::data(const QVariantList &indexPath) {
    if (indexPath.length() == 1) {
        QVariantMap map = taskDB.value(indexPath.value(0).toInt(NULL)).toMap();
        return QVariant(map);
    } else if (indexPath.length() == 2) {
        //Only used to return attachment property of tasks
        QVariantMap map = taskDB.value(indexPath.value(0).toInt(NULL)).toMap();
        QVariantMap attachments = map["attachments"].toList().value(indexPath.value(1).toInt(NULL)).toMap();
        return QVariant(attachments);
    }
    return QVariant();
}
/*
 * End of functions required by DataModel
 */

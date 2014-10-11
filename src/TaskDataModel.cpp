#include "TaskDataModel.hpp"
#include <bb/data/JsonDataAccess>

using namespace bb::cascades;
using namespace bb::data;

//Location where DB is stored by app on exit
const QString TaskDataModel::databasePath = QString("app/native/assets/data/tasks.json");

TaskDataModel::TaskDataModel(QObject *parent) : DataModel(parent) {
}
TaskDataModel::~TaskDataModel() {
    bb::data::JsonDataAccess jda;
    jda.save(this->internalDB, TaskDataModel::databasePath);
}

void TaskDataModel::initDatabase(const QString &filename) {
    bb::data::JsonDataAccess jda;
    bool loaded = false;

    if (QFile::exists(filename)) {
        qDebug() << Q_FUNC_INFO << "Found local database, loading.";
        this->internalDB = jda.load(filename).value<QVariantList>();
        if (jda.hasError()) {
            bb::data::DataAccessError e = jda.error();
            qDebug() << Q_FUNC_INFO << "JSON loading error: " << filename << e.errorType() << ": " << e.errorMessage();
        } else {
            loaded = true;
        }
    }

    if (!loaded) {
        qDebug() << Q_FUNC_INFO << "FAILED TO LOAD DATABASE";
    }
    sortTasksByDueDate();
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
    qSort(this->internalDB.begin(), this->internalDB.end(), compareTasksByDueDate);
}

/*
 * Slots
 */
void TaskDataModel::onTaskEdited(QVariantMap task) {
    //Find task in datamodel, if it exists
    for (int i = 0; i < this->internalDB.count(); ++i) {
        if (this->internalDB.value(i).toMap().value("id").toLongLong(NULL) == task.value("id").toLongLong(NULL)) {
            //Check if the task has been completed
            if (task.value("completed").toLongLong(NULL) == 0) {
                this->internalDB.replace(i, task);
                sortTasksByDueDate();
                emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
                qDebug() << Q_FUNC_INFO << "Task edited in TaskDataModel:" << task;
            } else {
                //If the task has a completion date, remove it from the datamodel
                onTaskRemoved(task);
            }
            return;
        }
    }

    //If not found in datamodel, then add as a new task
    onTaskAdded(task);
}

void TaskDataModel::onTaskAdded(QVariantMap task) {
    //Just add task to end of list and re-sort list
    this->internalDB.append(task);

    sortTasksByDueDate();
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
    qDebug() << Q_FUNC_INFO << "New task added to TaskDataModel:" << task;
}

void TaskDataModel::onTaskRemoved(QVariantMap task) {
    for (int i = 0; i < this->internalDB.count(); ++i) {
        if (this->internalDB.value(i).toMap().value("id").toLongLong(NULL) == task.value("id").toLongLong(NULL)) {
            this->internalDB.removeAt(i);
            sortTasksByDueDate();
            emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
            qDebug() << Q_FUNC_INFO << "Task removed from TaskDataModel:" << task;
            return;
        }
    }
}

void TaskDataModel::onLoggedOut() {
    //Clear all stored tasks
    this->internalDB = QVariantList();
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
    bb::data::JsonDataAccess jda;
    jda.save(this->internalDB, TaskDataModel::databasePath);
}
/*
 * End slots
 */

/*
 * Functions required by DataModel. Not sure if these work.
 */
int TaskDataModel::childCount(const QVariantList &indexPath) {
    if (indexPath.length() == 0) {
        return this->internalDB.length();
    } else if (indexPath.length() == 1) {
        //Only a task's attachment property has children
        QVariantMap map = this->internalDB.value(indexPath.value(0).toInt(NULL)).toMap();
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
        QVariantMap map = this->internalDB.value(indexPath.value(0).toInt(NULL)).toMap();
        qDebug() << Q_FUNC_INFO << indexPath << map;
        return QVariant(map);
    } else if (indexPath.length() == 2) {
        //Only used to return attachment property of tasks
        QVariantMap map = this->internalDB.value(indexPath.value(0).toInt(NULL)).toMap();
        QVariantMap attachments = map["attachments"].toList().value(indexPath.value(1).toInt(NULL)).toMap();
        return QVariant(attachments);
    }
    return QVariant();
}
/*
 * End of functions required by DataModel
 */

#include "TaskDataModel.hpp"
#include <bb/data/JsonDataAccess>

using namespace bb::cascades;
using namespace bb::data;

//Location where DB is stored by app on exit
const QString TaskDataModel::databasePath = QString("app/native/assets/data/tasks.json");

TaskDataModel::TaskDataModel(QObject *parent) : DataModel(parent) {
//    this->initDatabase(TaskDataModel::databasePath);
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
    } else {
        return first.value("duedate").toLongLong(NULL) < second.value("duedate").toLongLong(NULL);
    }
}

void TaskDataModel::sortTasksByDueDate() {
    qSort(this->internalDB.begin(), this->internalDB.end(), compareTasksByDueDate);
}

void TaskDataModel::addTask(QVariantMap taskData) {
    //Task added in UI, need to add to datamodel

    //A bunch of values come in as non-qlonglong values, convert them where applicable
    foreach (QString k, taskData.keys()) {
        if (taskData[k].canConvert(QVariant::LongLong) && taskData[k].type() != QVariant::String) {
            taskData[k] = QVariant(taskData[k].toLongLong(NULL));
        }
    }

    emit taskAdded(QVariantList() << taskData); //Causes TaskSenderReceiver to push changes
}

void TaskDataModel::editTask(QVariantMap taskData) {
    //Task edited in UI, need to update datamodel

    //A bunch of values come in as non-qlonglong values, convert them where applicable
    foreach (QString k, taskData.keys()) {
        if (taskData[k].canConvert(QVariant::LongLong) && taskData[k].type() != QVariant::String) {
            taskData[k] = QVariant(taskData[k].toLongLong(NULL));
        }
    }

    //TODO: Improve efficiency here, so that only changed fields get updated
    for (int i = 0; i < this->internalDB.count(); ++i) {
        QVariantMap internalTask = this->internalDB.value(i).toMap();
        if (internalTask["id"].toLongLong(NULL) == taskData["id"]) {
            this->internalDB.replace(i, taskData);
            emit itemUpdated(QVariantList() << i); //Causes ListView to update
            emit taskEdited(QVariantList() << taskData); //Causes TaskSenderReceiver to push changes
            break;
        }
    }

//    sortTasksByDueDate();
}

void TaskDataModel::removeTask(QVariantMap taskData) {
    for (int i = 0; i < this->internalDB.count(); ++i) {
        QVariantMap task = this->internalDB.value(i).toMap();
        if (task["id"].toLongLong(NULL) == taskData["id"]) {
            this->internalDB.removeAt(i);
            emit itemRemoved(QVariantList() << i); //Causes ListView to update
            emit taskRemoved(QVariantList() << taskData); //Causes TaskSenderReceiver to push changes
            break;
        }
    }
}

void TaskDataModel::onTasksUpdated(QVariantList tasks) {
    //Task updates received from TaskSenderReceiver, update datamodel with changes

    //TODO: Make this more general, using the following
//    //Cycle through received tasks and update data model if said tasks already
//    //exist, or add them to the end if they don't
//    for (int i = 0; i < tasks.count(); ++i) {
//        QVariantMap receivedTask = tasks.value(i).toMap();
//        bool inserted = false;
//        for (int j = 0; j < this->internalDB.count(); ++j) {
//            QVariantMap internalTask = this->internalDB.value(j).toMap();
//
//            if (internalTask["id"].toLongLong(NULL) == receivedTask["id"].toLongLong(NULL)) {
//                this->internalDB.replace(j, receivedTask);
//                inserted = true;
//                break;
//            }
//        }
//        //If task ID didn't already exist, task is new, so add to end
//        if (!inserted) {
//            this->internalDB.append(receivedTask);
//        }
//    }
    this->internalDB = tasks;

    sortTasksByDueDate();
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
    qDebug() << Q_FUNC_INFO << "Tasks in TaskDataModel updated from received tasks";
}

void TaskDataModel::onTasksAdded(QVariantList tasks) {
    //Task additions received from TaskSenderReceiver, update datamodel with new tasks

    //Just add tasks to end of list and re-sort list
    for (int i = 0; i < tasks.count(); ++i) {
        this->internalDB.append(tasks.value(i));
    }

    sortTasksByDueDate();
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
    qDebug() << Q_FUNC_INFO << "New tasks added to TaskDataModel";
}

void TaskDataModel::onTasksRemoved(QVariantList taskIDs) {
    //Tasks removals received from TaskSenderReceiver, update datamodel with removed tasks
    //TODO: This.
}

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
        QVariantMap map = this->internalDB.value(indexPath.value(0).toInt(NULL)).toMap();
        if (map.contains("timecategory")) {
            return QString("header");
        }
        return QString("task");
    }
    return QString::null;
}
QVariant TaskDataModel::data(const QVariantList &indexPath) {
    if (indexPath.length() == 1) {
        QVariantMap map = this->internalDB.value(indexPath.value(0).toInt(NULL)).toMap();
        return QVariant(map);
    } else if (indexPath.length() == 2) {
        //Only used to return attachment property of tasks
        QVariantMap map = this->internalDB.value(indexPath.value(0).toInt(NULL)).toMap();
        QVariantMap attachments = map["attachments"].toList().value(indexPath.value(1).toInt(NULL)).toMap();
        return QVariant(attachments);
    }
    return QVariant();
}

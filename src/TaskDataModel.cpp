#include "TaskDataModel.hpp"
#include <bb/data/JsonDataAccess>

using namespace bb::cascades;
using namespace bb::data;

//Location where DB is stored by app on exit
const QString TaskDataModel::databasePath = QString("app/native/assets/data/tasks.json");

TaskDataModel::TaskDataModel(QObject *parent) : DataModel(parent) {
    this->initDatabase(TaskDataModel::databasePath);
}
TaskDataModel::~TaskDataModel() {
    bb::data::JsonDataAccess jda;
    jda.save(this->internalDB, TaskDataModel::databasePath);
}

void TaskDataModel::initDatabase(const QString &filename) {
    bb::data::JsonDataAccess jda;
    bool loaded = false;

    if (QFile::exists(filename)) {
        qDebug() << "Found local database, loading.";
        this->internalDB = jda.load(filename).value<QVariantList>();
        if (jda.hasError()) {
            bb::data::DataAccessError e = jda.error();
            qDebug() << "JSON loading error: " << filename << e.errorType() << ": " << e.errorMessage();
        } else {
            loaded = true;
        }
    }

    if (!loaded) {
        qDebug() << "FAILED TO LOAD DATABASE";
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

void TaskDataModel::onTaskAdded(QVariantMap taskData) {
    //A bunch of values come in as non-qlonglong values, convert them where applicable
    foreach (QString k, taskData.keys()) {
        if (taskData[k].canConvert(QVariant::LongLong) && taskData[k].type() != QVariant::String) {
            taskData[k] = QVariant(taskData[k].toLongLong(NULL));
        }
    }
    this->internalDB.append(taskData);
    sortTasksByDueDate();
    emit itemAdded(QVariantList() << this->internalDB.count()-1);
}

void TaskDataModel::onTaskEdited(QVariantMap taskData) {
    //A bunch of values come in as non-qlonglong values, convert them where applicable
    foreach (QString k, taskData.keys()) {
        if (taskData[k].canConvert(QVariant::LongLong) && taskData[k].type() != QVariant::String) {
            taskData[k] = QVariant(taskData[k].toLongLong(NULL));
        }
    }
    for (int i = 0; i < this->internalDB.count(); ++i) {
        QVariantMap taskInfo = this->internalDB.value(i).toMap();
        if (taskInfo["id"].toLongLong(NULL) == taskData["id"]) {
            this->internalDB.replace(i, taskData);
            sortTasksByDueDate();
            emit itemUpdated(QVariantList() << i);
            break;
        }
    }
}

void TaskDataModel::onTasksUpdated(QVariantList tasks) {
    this->internalDB = tasks;
    sortTasksByDueDate();
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
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

#include "TaskDataModel.hpp"
#include <bb/data/JsonDataAccess>
#include <bb/cascades/ListView>

using namespace bb::cascades;
using namespace bb::data;

//Location where DB is stored by app on exit
const QString TaskDataModel::databasePath = QString("app/native/assets/data/tasks.json");
const QString TaskDataModel::demoDatabasePath = QString("app/native/assets/data/demoTasks.json");

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
        qDebug() << "--> Found local database, loading.";
        this->internalDB = jda.load(filename).value<QVariantList>();
        if (jda.hasError()) {
            bb::data::DataAccessError e = jda.error();
            qDebug() << "--> JSON loading error: " << filename << e.errorType() << ": " << e.errorMessage();
        } else {
            loaded = true;
        }
    }

    if (!loaded) {
        qDebug() << "--> Error loading stored database, loading demo database from" << TaskDataModel::demoDatabasePath;
        this->internalDB = jda.load(TaskDataModel::demoDatabasePath).value<QVariantList>();
        if (jda.hasError()) {
            bb::data::DataAccessError e = jda.error();
            qDebug() << filename << " JSON loading error:" << e.errorType() << ": " << e.errorMessage();
        } else {
            loaded = true;
        }
    }

    if (!loaded) {
        qDebug() << "--> FAILED TO LOAD DATABASE";
    }
    qDebug() << "--> Database:" << this->internalDB;
}

void TaskDataModel::onTaskAdded(QVariantMap taskData) {
    qDebug() << "--> Adding task to TaskDataModel...";
    qDebug() << taskData;
    this->internalDB.append(taskData);
    emit itemAdded(QVariantList() << this->internalDB.count()-1);
    qDebug() << "--> Task added:" << this->internalDB.at(this->internalDB.count()-1);
}

void TaskDataModel::onTaskEdited(QVariantMap taskData) {
    qDebug() << "--> Editing task in TaskDataModel...";
    //A bunch of values come in as non-qlonglong values, convert them where applicable
    qDebug() << taskData;
    foreach (QVariant v, taskData.values()) {
        if (v.canConvert(QVariant::LongLong) && v.type() != QVariant::String) {
            qDebug() << "converting" << v;
            v.convert(QVariant::LongLong);
            qDebug() << v;
        }
    }
    //Convert the id into an int (remove when ID is not visible)
    taskData["id"] = taskData["id"].toLongLong(NULL);

    qDebug() << "---> Editing task with ID" << taskData["id"];
    for (int i = 0; i < this->internalDB.count(); ++i) {
        QVariantMap taskInfo = this->internalDB.value(i).toMap();
        qDebug() << "---> Checking task" << taskInfo["title"] << "with ID" << taskInfo["id"] << "...";
        if (taskInfo["id"].toLongLong(NULL) == taskData["id"]) {
            this->internalDB.replace(i, taskData);
            emit itemUpdated(QVariantList() << i);
            qDebug() << "--> Task edited:" << this->internalDB.at(i);
            break;
        }
    }
}

void TaskDataModel::onLocalTasksRemoved() {
    //Clears local task storage
    bool isOk = QFile::remove(TaskDataModel::databasePath);
    if (!isOk) {
        qDebug() << "--> Couldn't remove local tasks!";
    } else {
        TaskDataModel::initDatabase(TaskDataModel::databasePath);
    }
}

QList<qlonglong> TaskDataModel::tasks() {
    //Returns a QList of ints of all Task IDs currently stored.
    QList<qlonglong> l;
    for (int i = 0; i < this->internalDB.count(); ++i) {
        QVariantMap taskInfo = this->internalDB.value(i).toMap();
        l << taskInfo["id"].toLongLong(NULL);
    }
    return l;
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

void TaskDataModel::removeItems(const QVariantList &indexPaths) {
    for (int i = indexPaths.count() - 1; i >= 0; --i) {
        QVariant indexPath = indexPaths.value(i);
        QVariantList indexPathList = indexPath.toList();
        if (indexPathList.count() != 1) {
            //Not a proper indexPath for this datatype
            continue;
        }
        int index = indexPathList.value(0).toInt(NULL);
        this->internalDB.removeAt(index);
        emit itemRemoved(indexPathList);
    }
}


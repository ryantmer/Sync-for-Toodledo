#include "TaskDataModel.hpp"
#include <bb/data/JsonDataAccess>
#include <bb/cascades/ListView>

using namespace bb::cascades;
using namespace bb::data;

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
            qDebug() << filename << " JSON loading error: " << e.errorType() << ": " << e.errorMessage();
        } else {
            loaded = true;
        }
    }

    if (!loaded) {
        qDebug() << "Error loading database, loading default database.";
        this->internalDB = jda.load("app/native/assets/data/demoTasks.json").value<QVariantList>();
        if (jda.hasError()) {
            bb::data::DataAccessError e = jda.error();
            qDebug() << filename << " JSON loading error: " << e.errorType() << ": " << e.errorMessage();
        } else {
            loaded = true;
        }
    }

    if (!loaded) {
        qDebug() << "FAILED TO LOAD DATABASE";
    }
    qDebug() << "Database: " << this->internalDB;
}

void TaskDataModel::updateDatabase() {
    for (int i = 1; i < this->internalDB.count(); ++i) {
        QVariantMap taskInfo = this->internalDB.value(i).toMap();
        this->internalDB.replace(i, taskInfo);
        //emit DataModel signal that an item has been updated (with item index)
        emit itemUpdated(QVariantList() << i);
    }
}

QVariantList TaskDataModel::tasks() {
    //Returns a QVariantList of all Task IDs currently stored.
    QVariantList l;
    for (int i = 0; i < this->internalDB.count(); ++i) {
        QVariantMap taskInfo = this->internalDB.value(i).toMap();
        l << taskInfo["id"];
    }
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

void TaskDataModel::addTask(QVariantMap data) {
    this->internalDB.append(data);
    emit itemAdded(QVariantList() << this->internalDB.count() - 1);
}

//void TaskDataModel::onTaskUpdated(int id, QVariantMap data) {
//    for (int i = 0; i < this->internalDB.count(); ++i) {
//        QVariantMap taskInfo = this->internalDB.value(i).toMap();
//        if (taskInfo["id"].toInt(NULL) == id) {
//            QList<QString> keys = data.keys();
//            foreach(QString key, keys) {
//                taskInfo.insert(key, data.value(key));
//            }
//            this->internalDB.replace(i, data);
//            emit itemUpdated(QVariantList() << i);
//            break;
//        }
//    }
//}

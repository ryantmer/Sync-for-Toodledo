#include "FolderDataModel.hpp"
#include <bb/data/JsonDataAccess>

using namespace bb::cascades;
using namespace bb::data;

FolderDataModel::FolderDataModel(QObject *parent) : DataModel(parent) {}

FolderDataModel::~FolderDataModel() {}

void FolderDataModel::clear() {
    folderDB.clear();
}

QVariantList FolderDataModel::getFolderList() {
    return folderDB;
}

bool compareFoldersByOrd(QVariant &a, QVariant &b) {
    QVariantMap first = a.toMap();
    QVariantMap second = b.toMap();

    return first.value("ord").toLongLong(NULL) < second.value("ord").toLongLong(NULL);
}

void FolderDataModel::sortFoldersByOrd() {
    qSort(folderDB.begin(), folderDB.end(), compareFoldersByOrd);
}

/*
 * Slots
 */
void FolderDataModel::onFolderEdited(QVariantMap folder) {
    //Find folder in datamodel, if it exists
    for (int i = 0; i < folderDB.count(); ++i) {
        if (folderDB.value(i).toMap().value("id").toLongLong(NULL) == folder.value("id").toLongLong(NULL)) {
            //Replace values in local folder with new values, by key
            QVariantMap localFolder = folderDB.value(i).toMap();
            for (QVariantMap::const_iterator iter = folder.begin(); iter != folder.end(); ++iter) {
                localFolder.insert(iter.key(), iter.value());
            }
            folderDB.replace(i, localFolder);

            emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
            qDebug() << Q_FUNC_INFO << "Folder edited in FolderDataModel:" << folder;
            return;
        }
    }

    //If not found in datamodel, then add as a new folder
    onFolderAdded(folder);
}

void FolderDataModel::onFolderAdded(QVariantMap folder) {
    if (folder.value("archived").toInt(NULL) == 0) {
        folderDB.append(folder);
        emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
        qDebug() << Q_FUNC_INFO << "New folder added to FolderDataModel:" << folder;
    } else {
        //Don't add archived folders
        return;
    }
}

void FolderDataModel::onFolderRemoved(QVariantMap folder) {
    for (int i = 0; i < folderDB.count(); ++i) {
        if (folderDB.value(i).toMap().value("id").toLongLong(NULL) == folder.value("deleted").toLongLong(NULL)) {
            folderDB.removeAt(i);
            emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
            qDebug() << Q_FUNC_INFO << "Folder removed from FolderDataModel:" << folder;
            return;
        }
    }
}

void FolderDataModel::onLoggedOut() {
    //Clear all stored folders
    folderDB = QVariantList();
    emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
}
/*
 * End slots
 */

/*
 * Functions required by DataModel. Not sure if these work.
 */
int FolderDataModel::childCount(const QVariantList &indexPath) {
    if (indexPath.length() == 0) {
        return folderDB.length();
    }
    return 0;
}
bool FolderDataModel::hasChildren(const QVariantList &indexPath) {
    if (indexPath.length() == 0) {
        return true;
    } else if (indexPath.length() == 1 && itemType(indexPath) == QString("folder")) {
        return true;
    }
    return false;
}
QString FolderDataModel::itemType(const QVariantList &indexPath) {
    if (indexPath.length() == 1) {
        return QString("folder");
    }
    return QString::null;
}
QVariant FolderDataModel::data(const QVariantList &indexPath) {
    if (indexPath.length() == 1) {
        QVariantMap map = folderDB.value(indexPath.value(0).toInt(NULL)).toMap();
        return QVariant(map);
    } else if (indexPath.length() == 2) {
        //Only used to return attachment property of folders
        QVariantMap map = folderDB.value(indexPath.value(0).toInt(NULL)).toMap();
        QVariantMap attachments = map["attachments"].toList().value(indexPath.value(1).toInt(NULL)).toMap();
        return QVariant(attachments);
    }
    return QVariant();
}
/*
 * End of functions required by DataModel
 */

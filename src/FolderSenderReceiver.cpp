#include <bb/data/JsonDataAccess>
#include "FolderSenderReceiver.hpp"

const QString FolderSenderReceiver::getUrl = QString("http://api.toodledo.com/3/folders/get.php");
const QString FolderSenderReceiver::editUrl = QString("http://api.toodledo.com/3/folders/edit.php");
const QString FolderSenderReceiver::addUrl = QString("http://api.toodledo.com/3/folders/add.php");
const QString FolderSenderReceiver::removeUrl = QString("http://api.toodledo.com/3/folders/delete.php");

using namespace bb::data;

FolderSenderReceiver::FolderSenderReceiver(QObject *parent) : QObject(parent) {
    _networkAccessManager = new QNetworkAccessManager(this);
    _propMan = PropertiesManager::getInstance();

    bool isOk;
    isOk = connect(_networkAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onReplyReceived(QNetworkReply*)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);
}

void FolderSenderReceiver::fetchAllFolders() {
    QUrl url(getUrl);
    url.addQueryItem("access_token", _propMan->accessToken);
    QNetworkRequest req(url);
    _networkAccessManager->get(req);
}

/*
 * Slots
 */
void FolderSenderReceiver::onFolderAdded(QVariantMap folder) {
    //Adding folders not currently supported
    Q_UNUSED(folder);
}

void FolderSenderReceiver::onFolderEdited(QVariantMap oldData, QVariantMap newData) {
    //Editing folders not currently supported
    Q_UNUSED(oldData);
    Q_UNUSED(newData);
}

void FolderSenderReceiver::onFolderRemoved(QVariantMap folder) {
    //Removing folders not currently supported
    Q_UNUSED(folder);
}

void FolderSenderReceiver::onReplyReceived(QNetworkReply *reply) {
    QString response = reply->readAll();

    JsonDataAccess jda;
    QVariantList data = jda.loadFromBuffer(response).value<QVariantList>();
    if (jda.hasError()) {
        qDebug() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
        qDebug() << Q_FUNC_INFO << response;
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        if (reply->url().toString().contains(getUrl)) {
            qDebug() << Q_FUNC_INFO << "Folder(s) received";
            for (int i = 0; i < data.count(); ++i) {
                emit folderGetReply(data.value(i).toMap());
            }
        } else if (reply->url().toString().contains(addUrl)) {
            qDebug() << Q_FUNC_INFO << "New folder(s) added";
            for (int i = 0; i < data.count(); ++i) {
                emit folderAddReply(data.value(i).toMap());
            }
        } else if (reply->url().toString().contains(removeUrl)) {
            qDebug() << Q_FUNC_INFO << "Folder(s) removed";
            for (int i = 0; i < data.count(); ++i) {
                emit folderRemoveReply(data.value(i).toMap());
            }
        } else if (reply->url().toString().contains(editUrl)) {
            qDebug() << Q_FUNC_INFO << "Folder(s) edited";
            for (int i = 0; i < data.count(); ++i) {
                emit folderEditReply(data.value(i).toMap());
            }
        } else {
            qDebug() << Q_FUNC_INFO << "Unrecognized reply received:" << data;
        }
    } else {
        //ToodleDo will come back with various error codes if there's a problem
        QVariantMap errorMap = jda.loadFromBuffer(response).value<QVariantMap>();
        if (jda.hasError()) {
            qDebug() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
            qDebug() << Q_FUNC_INFO << response;
            return;
        }

        qDebug() << Q_FUNC_INFO << "ToodleDo error" <<
                errorMap.value("errorCode").toInt(NULL) << ":" <<
                errorMap.value("errorDesc").toString();
    }
    reply->deleteLater();
}
/*
 * End slots
 */

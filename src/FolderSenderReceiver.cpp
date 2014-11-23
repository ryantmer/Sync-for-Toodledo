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
    QUrl url(addUrl);
    QNetworkRequest req(url);

    QUrl data;
    data.addQueryItem("access_token", _propMan->accessToken);
    if (folder["name"].toString() != "") {
        data.addQueryItem("name", folder["name"].toString());
    } else {
        //Can't add a folder without a name
        //This should never be hit, as QML doesn't allow adding without a name
        qDebug() << Q_FUNC_INFO << "Can't add folder without name! (you shouldn't ever see this)";
        return;
    }
    if (folder["private"].toInt(NULL) == 1) {
        data.addQueryItem("private", folder["private"].toString());
    }

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    _networkAccessManager->post(req, data.encodedQuery());
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

    if (reply->error() == QNetworkReply::NoError) {
        JsonDataAccess jda;
        QVariantList data = jda.loadFromBuffer(response).value<QVariantList>();
        if (jda.hasError()) {
            qDebug() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
            qDebug() << Q_FUNC_INFO << response;
            return;
        }

        //If the reply can't be interpreted as a List, try as a Map (errors come back in this format)
        if (data.length() == 0) {
            QVariantMap errorMap = jda.loadFromBuffer(response).value<QVariantMap>();
            if (jda.hasError()) {
                qDebug() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
                qDebug() << Q_FUNC_INFO << response;
                return;
            }
            qDebug() << Q_FUNC_INFO << "ToodleDo error" <<
                        errorMap.value("errorCode").toInt(NULL) << ":" <<
                        errorMap.value("errorDesc").toString();
            emit toast("Toodledo Error " + errorMap.value("errorCode").toString() +
                        " : " + errorMap.value("errorDesc").toString());
            return;
        }

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
        qDebug() << Q_FUNC_INFO << response;
        emit toast("Network error!");
    }
    reply->deleteLater();
}
/*
 * End slots
 */

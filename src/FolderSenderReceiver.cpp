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
    QUrl url(editUrl);
    QNetworkRequest req(url);

    if (oldData == newData) {
        //If nothing has changed, don't need to upload anything
        return;
    }

    QUrl data;
    data.addQueryItem("access_token", _propMan->accessToken);
    data.addQueryItem("id", newData["id"].toString());
    QVariantMap::iterator i;
    for (i = newData.begin(); i != newData.end(); ++i) {
        if (oldData[i.key()] != newData[i.key()]) {
            data.addQueryItem(i.key(), newData[i.key()].toString());
        }
    }

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    _networkAccessManager->post(req, data.encodedQuery());
}

void FolderSenderReceiver::onFolderRemoved(QVariantMap folder) {
    QUrl url(removeUrl);
    QNetworkRequest req(url);

    QUrl data;
    data.addQueryItem("access_token", _propMan->accessToken);
    data.addQueryItem("id", folder["id"].toString());

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    _networkAccessManager->post(req, data.encodedQuery());
}

void FolderSenderReceiver::onReplyReceived(QNetworkReply *reply) {
    QString response = reply->readAll();

    if (reply->error() == QNetworkReply::NoError) {
        /* Reply handling here looks waaaaay too complicated, but it's because
         * replies come in one of three formats: a single map, a list of maps
         * with only one entry, or a list of maps with multiple entries. Way
         * to be consistent, Toodledo API...
         */
        JsonDataAccess jda;
        QVariantList dataList = jda.loadFromBuffer(response).value<QVariantList>();
        if (jda.hasError()) {
            qDebug() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
            qDebug() << Q_FUNC_INFO << response;
            return;
        }

        QVariantMap dataMap = jda.loadFromBuffer(response).value<QVariantMap>();
        if (jda.hasError()) {
            qDebug() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
            qDebug() << Q_FUNC_INFO << response;
            return;
        }

        if (dataMap.contains("errorDesc")) {
            qDebug() << Q_FUNC_INFO << "Toodledo error" <<
                        dataMap.value("errorCode").toInt(NULL) << ":" <<
                        dataMap.value("errorDesc").toString();
            emit toast("Toodledo Error " + dataMap.value("errorCode").toString() +
                        " : " + dataMap.value("errorDesc").toString());
            return;
        }

        if (reply->url().toString().contains(getUrl)) {
            qDebug() << Q_FUNC_INFO << "Folder(s) received";
            for (int i = 0; i < dataList.count(); ++i) {
                emit folderGetReply(dataList.value(i).toMap());
            }
        } else if (reply->url().toString().contains(addUrl)) {
            qDebug() << Q_FUNC_INFO << "New folder(s) added";
            for (int i = 0; i < dataList.count(); ++ i) {
                emit folderAddReply(dataList.value(i).toMap());
            }
        } else if (reply->url().toString().contains(removeUrl)) {
            qDebug() << Q_FUNC_INFO << "Folder(s) removed";
            emit folderRemoveReply(dataMap);
        } else if (reply->url().toString().contains(editUrl)) {
            qDebug() << Q_FUNC_INFO << "Folder(s) edited";
            for (int i = 0; i < dataList.count(); ++i) {
                emit folderEditReply(dataList.value(i).toMap());
            }
        } else {
            qDebug() << Q_FUNC_INFO << "Unrecognized reply received:" << response;
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

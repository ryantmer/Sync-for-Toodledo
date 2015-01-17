#include <QMutex>
#include <QDebug>
#include <bb/data/JsonDataAccess>
#include "NetworkManager.hpp"

const QString NetworkManager::authorizeUrl = QString("https://api.toodledo.com/3/account/authorize.php");
const QString NetworkManager::tokenUrl = QString("https://api.toodledo.com/3/account/token.php");
const QString NetworkManager::getUrl = QString("http://api.toodledo.com/3/%1/get.php");
const QString NetworkManager::addUrl = QString("http://api.toodledo.com/3/%1/add.php");
const QString NetworkManager::editUrl = QString("http://api.toodledo.com/3/%1/edit.php");
const QString NetworkManager::removeUrl = QString("http://api.toodledo.com/3/%1/delete.php");
const QString NetworkManager::tasks = QString("tasks");
const QString NetworkManager::folders = QString("folders");
const QString NetworkManager::contexts = QString("contexts");
const QString NetworkManager::goals = QString("goals");
const QString NetworkManager::locations = QString("locations");
const QString NetworkManager::account = QString("account");

using namespace bb::data;

NetworkManager *NetworkManager::getInstance() {
    static NetworkManager *singleton = NULL;
    static QMutex mutex;
    if (singleton == NULL) {
        mutex.lock();
        if (!singleton) {
            singleton = new NetworkManager();
        }
        mutex.unlock();
    }
    return singleton;
}

NetworkManager::NetworkManager() {
    _netConfigManager = new QNetworkConfigurationManager(this);
    _netAccessManager = new QNetworkAccessManager(this);
    _connected = _netConfigManager->isOnline();

    bool ok;
    ok = connect(_netConfigManager, SIGNAL(onlineStateChanged(bool)),
            this, SLOT(onOnlineStateChanged(bool)));
    Q_ASSERT(ok);
    ok = connect(_netAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onFinished(QNetworkReply*)));
    Q_UNUSED(ok);
}
NetworkManager::~NetworkManager() {}

bool NetworkManager::isConnected() {
    return _connected;
}

void NetworkManager::onOnlineStateChanged(bool online) {
    qDebug() << Q_FUNC_INFO << "Network state changed. Online:" << online;
    if (online) {
        _connected = true;
    } else {
        _connected = false;
    }
    emit networkStateChanged(_connected);
}

void NetworkManager::sendRequest(QNetworkRequest request, QByteArray encodedQuery) {
    qDebug() << Q_FUNC_INFO << "Sending" << encodedQuery << "to" << request.url().toString();
    emit networkRequestStarted(); //Sets activity indicator visible
    _netAccessManager->post(request, encodedQuery);
}

void NetworkManager::onFinished(QNetworkReply *reply) {
    QString response = reply->readAll();

    qDebug() << Q_FUNC_INFO << response;

    JsonDataAccess jda;
    //Some replies are a list of maps, others are just a single map
    QVariantMap dataMap = jda.loadFromBuffer(response).value<QVariantMap>();
    if (jda.hasError()) {
        qWarning() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
        qWarning() << Q_FUNC_INFO << response;
        return;
    }
    QVariantList dataList = jda.loadFromBuffer(response).value<QVariantList>();
    if (jda.hasError()) {
        qWarning() << Q_FUNC_INFO << "Error reading network response into JSON:" << jda.error();
        qWarning() << Q_FUNC_INFO << response;
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        if (dataMap.contains("errorDesc")) {
            qWarning() << Q_FUNC_INFO << "Toodledo error" <<
                        dataMap.value("errorCode").toInt(NULL) << ":" <<
                        dataMap.value("errorDesc").toString();
            emit toast("Error " + dataMap.value("errorCode").toString() +
                    ": " + dataMap.value("errorDesc").toString());
            return;
        }

        QString replyUrl = reply->url().toString(QUrl::RemoveQuery);

        if (replyUrl == tokenUrl) {
            qDebug() << Q_FUNC_INFO << "New refresh token received:" << dataMap.value("refresh_token").toString();
            emit refreshTokenRefreshed(dataMap.value("refresh_token").toString());
            qDebug() << Q_FUNC_INFO << "New access token received:" << dataMap.value("access_token").toString();
            emit accessTokenRefreshed(dataMap.value("access_token").toString(),
                    dataMap.value("expires_in").toLongLong(NULL));
        } else if (replyUrl.contains(tasks)) {
            if (replyUrl.contains("get")) {
                if (dataList.length() > 0) {
                    dataList.pop_front(); //Discard summary item (only for getting tasks)
                }
                emit taskGetReply(dataList);
            } else if (replyUrl.contains("add")) {

            } else if (replyUrl.contains("edit")) {

            } else if (replyUrl.contains("remove")) {

            } else {
                qDebug() << Q_FUNC_INFO << "Received unknown reply from task URL:" << response;
            }
        } else if (replyUrl.contains(folders)) {

        }
//        else if (replyUrl == getUrl.arg(tasks) || replyUrl == getUrl.arg(folders) ||
//                replyUrl == getUrl.arg(contexts) || replyUrl == getUrl.arg(goals) ||
//                replyUrl == getUrl.arg(locations)) {
//            qDebug() << Q_FUNC_INFO << "Get URL data received from" << replyUrl << ":" << dataList;
//            //Discard summary item (only included when getting tasks)
//            if (dataList.length() > 0) {
//                if (dataList.first().toMap().contains("num") && dataList.first().toMap().contains("total")) {
//                    dataList.pop_front();
//                }
//            }
//            addToDataModel(dataList);
//        }
//        else if (replyUrl == getUrl.arg(account)) {
//            qDebug() << Q_FUNC_INFO << "Account data received from" << replyUrl << ":" << dataMap;
//            addToDataModel(QVariantList() << dataMap);
//        }
//        else if (replyUrl == addUrl.arg(tasks) || replyUrl == addUrl.arg(folders) ||
//                replyUrl == addUrl.arg(contexts) || replyUrl == addUrl.arg(goals) ||
//                replyUrl == addUrl.arg(locations)) {
//            qDebug() << Q_FUNC_INFO << "Add URL data received from" << replyUrl << ":" << dataList;
//            addToDataModel(dataList);
//        }
//        else if (replyUrl == editUrl.arg(tasks) || replyUrl == editUrl.arg(folders) ||
//                replyUrl == editUrl.arg(contexts) || replyUrl == editUrl.arg(goals) ||
//                replyUrl == editUrl.arg(locations)) {
//            qDebug() << Q_FUNC_INFO << "Edit URL data received from" << replyUrl << ":" << dataList;
//            editInDataModel(dataList);
//        }
//        else if (replyUrl == removeUrl.arg(tasks)) {
//            qDebug() << Q_FUNC_INFO << "Remove URL data received from" << replyUrl << ":" << dataList;
//            removeFromDataModel(dataList);
//        }
//        else if (replyUrl == removeUrl.arg(folders) || replyUrl == removeUrl.arg(contexts) ||
//                replyUrl == removeUrl.arg(goals) || replyUrl == removeUrl.arg(locations)) {
//            //These replies come as a single map, not list of maps
//            qDebug() << Q_FUNC_INFO << "Remove URL data received from" << replyUrl << ":" << dataMap;
//            removeFromDataModel(QVariantList() << dataMap);
//        }
//        else {
//            qWarning() << Q_FUNC_INFO << "Unrecognized reply received from" << replyUrl;
//            qWarning() << Q_FUNC_INFO << response;
//        }
    } else {
        qWarning() << Q_FUNC_INFO << "Reply from" << reply->url() << "contains error" << reply->errorString();
        qWarning() << Q_FUNC_INFO << response;
        emit toast(QString("Error %1: %2").arg(dataMap.value("errorCode").toString(),
                dataMap.value("errorDesc").toString()));
    }
    reply->deleteLater();
    emit networkRequestFinished(); //hides activity indicator
}

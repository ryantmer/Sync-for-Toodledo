#ifndef SENDERRECEIVER_HPP_
#define SENDERRECEIVER_HPP_

#include <QtNetwork>
#include "PropertiesManager.hpp"

class SenderReceiver : public QObject {
    Q_OBJECT

public:
    SenderReceiver(QObject *parent = 0);
    void fetchAllTasks();
    void fetchAllFolders();

    static const QString taskGetUrl;
    static const QString taskEditUrl;
    static const QString taskAddUrl;
    static const QString taskRemoveUrl;
    static const QString folderGetUrl;
    static const QString folderEditUrl;
    static const QString folderAddUrl;
    static const QString folderRemoveUrl;

signals:
    void taskEditReply(QVariantMap task);
    void taskAddReply(QVariantMap task);
    void taskRemoveReply(QVariantMap task);
    void taskGetReply(QVariantMap task);
    void folderEditReply(QVariantMap folder);
    void folderAddReply(QVariantMap folder);
    void folderRemoveReply(QVariantMap folder);
    void folderGetReply(QVariantMap folder);
    void toast(QString message);

public slots:
    void onTaskAdded(QVariantMap data);
    void onTaskEdited(QVariantMap oldData, QVariantMap newData);
    void onTaskRemoved(QVariantMap data);
    void onFolderAdded(QVariantMap data);
    void onFolderEdited(QVariantMap oldData, QVariantMap newData);
    void onFolderRemoved(QVariantMap data);
    void onReplyReceived(QNetworkReply *reply);

private:
    QNetworkAccessManager *_networkAccessManager;
    PropertiesManager *_propMan;
};

#endif /* SENDERRECEIVER_HPP_ */

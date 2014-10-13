#ifndef FOLDERSENDERRECEIVER_HPP_
#define FOLDERSENDERRECEIVER_HPP_

#include <QtNetwork>
#include "PropertiesManager.hpp"

class FolderSenderReceiver : public QObject {
    Q_OBJECT

public:
    FolderSenderReceiver(QObject *parent = 0);
    void fetchAllFolders();

    static const QString getUrl;
    static const QString editUrl;
    static const QString addUrl;
    static const QString removeUrl;

signals:
    void folderEditReply(QVariantMap folder);
    void folderAddReply(QVariantMap folder);
    void folderRemoveReply(QVariantMap folder);
    void folderGetReply(QVariantMap folder);

public slots:
    void onFolderAdded(QVariantMap data);
    void onFolderEdited(QVariantMap oldData, QVariantMap newData);
    void onFolderRemoved(QVariantMap data);
    void onReplyReceived(QNetworkReply *reply);

private:
    QNetworkAccessManager *_networkAccessManager;
    PropertiesManager *_propMan;
};

#endif /* FOLDERSENDERRECEIVER_HPP_ */

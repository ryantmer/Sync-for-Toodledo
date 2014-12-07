#ifndef SENDERRECEIVER_HPP_
#define SENDERRECEIVER_HPP_

#include <QtNetwork>
#include "PropertiesManager.hpp"

class SenderReceiver : public QObject {
    Q_OBJECT

public:
    enum DataType {
        UndefinedType,
        Task,
        Folder,
        CompletedTask,
        Context,
        Goal,
        Location
    };

    SenderReceiver(QObject *parent = 0);
    virtual ~SenderReceiver();

    void setDataType(DataType dataType);
    void fetchAll();

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
    void onAdd(QVariantMap data);
    void onEdit(QVariantMap oldData, QVariantMap newData);
    void onRemove(QVariantMap data);
    void onReplyReceived(QNetworkReply *reply);

private:
    DataType _dataType;
    QNetworkAccessManager *_networkAccessManager;
    PropertiesManager *_propMan;
};

#endif /* SENDERRECEIVER_HPP_ */

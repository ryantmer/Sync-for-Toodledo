#ifndef TASKRETRIEVER_HPP_
#define TASKRETRIEVER_HPP_

#include <QtNetwork>
#include "PropertiesManager.hpp"

class TaskSenderReceiver : public QObject {
    Q_OBJECT

public:
    TaskSenderReceiver(QObject *parent = 0);
    void fetchAllTasks();

    static const QString getUrl;
    static const QString editUrl;
    static const QString addUrl;
    static const QString removeUrl;

signals:
    void taskEditReply(QVariantMap task);
    void taskAddReply(QVariantMap task);
    void taskRemoveReply(QVariantMap task);
    void taskGetReply(QVariantMap task);
    void toast(QString message);

public slots:
    void onTaskAdded(QVariantMap data);
    void onTaskEdited(QVariantMap oldData, QVariantMap newData);
    void onTaskRemoved(QVariantMap data);
    void onReplyReceived(QNetworkReply *reply);

private:
    QNetworkAccessManager *_networkAccessManager;
    PropertiesManager *_propMan;
};

#endif /* TASKRETRIEVER_HPP_ */

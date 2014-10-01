#ifndef TASKRETRIEVER_HPP_
#define TASKRETRIEVER_HPP_

#include <QtNetwork>
#include "PropertiesManager.hpp"

class TaskSenderReceiver : public QObject {
    Q_OBJECT

public:
    TaskSenderReceiver(QObject *parent = 0);
    void fetchAllTasks();
    void fetchTask(int taskId);
    void addTasks(QVariantList task);
    void updateTasks(QVariantList task);

    static const QString getUrl;
    static const QString editUrl;
    static const QString addUrl;
    static const QString removeUrl;

signals:
    void tasksUpdated(QVariantList tasks);
    void tasksAdded(QVariantList tasks);
    void tasksRemoved(QVariantList taskIDs);

public slots:
    void onTaskAdded(QVariantList task);
    void onTaskEdited(QVariantList task);
    void onTaskRemoved(QVariantList task);
    void onTasksReceived(QNetworkReply *reply);

private:
    QNetworkAccessManager *_networkAccessManager;
    PropertiesManager *_propMan;
};

#endif /* TASKRETRIEVER_HPP_ */

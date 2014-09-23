#ifndef TASKRETRIEVER_HPP_
#define TASKRETRIEVER_HPP_

#include <QtNetwork>

class TaskRetriever : public QObject {
    Q_OBJECT

public:
    TaskRetriever(QObject *parent = 0);
    void fetchAllTasks();
    void fetchTask(int taskId);

    static const QString getUrl;

signals:
    void tasksUpdated(QVariantList data);

private slots:
    void onTasksReceived(QNetworkReply *reply);

private:
    QNetworkAccessManager *_retriever;
};

#endif /* TASKRETRIEVER_HPP_ */

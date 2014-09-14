#ifndef TASKRETRIEVER_HPP_
#define TASKRETRIEVER_HPP_

#include <QObject>
#include <QUrl>

class TaskRetriever : public QObject {
    Q_OBJECT

public:
    TaskRetriever(QObject *parent = 0);
    void fetchAllTasks();
    void fetchTask(int taskId);

signals:
    void tasksUpdated(QVariantList data);
    void taskUpdateFailed(int taskId);

private slots:
    void onNetworkResponse(QUrl url, QString response);
    void onNetworkResponseFailed(QUrl url, int error);

private:
};

#endif /* TASKRETRIEVER_HPP_ */

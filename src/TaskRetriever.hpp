#ifndef TASKRETRIEVER_HPP_
#define TASKRETRIEVER_HPP_

#include <QObject>
#include <QUrl>
#include <QVariant>

class TaskRetriever : public QObject {
    Q_OBJECT

public:
    TaskRetriever(QObject *parent = 0);
    void fetchAllTasks();
    void fetchTask(int taskId);

signals:
    void taskUpdated(int taskId, QVariantMap taskData);
    void taskUpdateFailed(int taskId);

private slots:
    void onNetworkResponse(QUrl url, QString response);
    void onNetworkResponseFailed(QUrl url, int error);

private:
    void parseTaskUpdateResponse(int taskId, QString response);
};

#endif /* TASKRETRIEVER_HPP_ */

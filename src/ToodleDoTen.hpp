#ifndef TOODLEDOTEN_HPP_
#define TOODLEDOTEN_HPP_

#include <QObject>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Page>

class TaskDataModel;
class TaskRetriever;
class ToodleDoTen : public QObject {
    Q_OBJECT

public:
    Q_PROPERTY(TaskDataModel *dataModel READ dataModel CONSTANT);

    ToodleDoTen();
    virtual ~ToodleDoTen();

    TaskDataModel *dataModel();

    Q_INVOKABLE QDateTime unixTimeToDateTime(uint unixTime);
    Q_INVOKABLE uint dateTimeToUnixTime(QDateTime dateTime);
    //These are called by various UI buttons
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void addTask(QVariant taskData);
    Q_INVOKABLE void editTask(QVariant taskData);
    Q_INVOKABLE void clearLocalTasks();

signals:
    void networkChanged();

private slots:
    void onTaskUpdated(QVariantMap taskData);
    void onNetworkStatusChanged(bool connected);
    void onRefreshTriggered();

private:
    TaskDataModel *_dataModel;
    TaskRetriever *_taskRetriever;
};

#endif /* TOODLEDOTEN_HPP_ */

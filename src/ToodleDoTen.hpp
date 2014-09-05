#ifndef TOODLEDOTEN_HPP_
#define TOODLEDOTEN_HPP_

#include <QObject>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/ActionItem>
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

    Q_INVOKABLE void updateDataModel();

signals:
    void networkChanged();

private slots:
    void onTaskUpdated(QVariantMap taskData);
    void onNetworkStatusChanged(bool connected);
    void onRefreshTriggered();

private:
    TaskDataModel *_dataModel;
    TaskRetriever *_taskRetriever;
    bb::cascades::ActionItem *_refreshButton;
    bb::cascades::ActionItem *_addTaskButton;
    bb::cascades::ActionItem *_editTaskButton;
};

#endif /* TOODLEDOTEN_HPP_ */

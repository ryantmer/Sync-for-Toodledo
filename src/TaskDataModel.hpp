#ifndef TASKDATAMODEL_HPP_
#define TASKDATAMODEL_HPP_

#include <QObject>
#include <bb/cascades/DataModel>

class TaskDataModel : public bb::cascades::DataModel {
    Q_OBJECT
    static const QString databasePath;

public:
    TaskDataModel(QObject *parent = 0);
    virtual ~TaskDataModel();

    //Required by bb::cascades::DataModel
    Q_INVOKABLE virtual int childCount(const QVariantList &indexPath);
    Q_INVOKABLE virtual bool hasChildren(const QVariantList &indexPath);
    Q_INVOKABLE virtual QString itemType(const QVariantList &indexPath);
    Q_INVOKABLE virtual QVariant data(const QVariantList &indexPath);

    void addTask(QVariantMap data);
    void editTask(QVariantMap data);

private:
    QVariantList internalDB;
    void initDatabase(const QString &filename);
    void sortTasksByDueDate();

public slots:
    void onTasksUpdated(QVariantList tasks);
};

#endif /* TASKDATAMODEL_HPP_ */

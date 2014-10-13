#ifndef TASKDATAMODEL_HPP_
#define TASKDATAMODEL_HPP_

#include <QObject>
#include <bb/cascades/DataModel>

class TaskDataModel : public bb::cascades::DataModel {
    Q_OBJECT
    static const QString taskDBPath;

public:
    TaskDataModel(QObject *parent = 0);
    virtual ~TaskDataModel();

    //Required by bb::cascades::DataModel
    Q_INVOKABLE virtual int childCount(const QVariantList &indexPath);
    Q_INVOKABLE virtual bool hasChildren(const QVariantList &indexPath);
    Q_INVOKABLE virtual QString itemType(const QVariantList &indexPath);
    Q_INVOKABLE virtual QVariant data(const QVariantList &indexPath);

private:
    QVariantList taskDB;
    void initDatabase();
    void sortTasksByDueDate();

signals:
    void toast(QString message);

public slots:
    void onTaskEdited(QVariantMap task);
    void onTaskAdded(QVariantMap task);
    void onTaskRemoved(QVariantMap task);
    void onLoggedOut();
    void onAboutToQuit();
};

#endif /* TASKDATAMODEL_HPP_ */

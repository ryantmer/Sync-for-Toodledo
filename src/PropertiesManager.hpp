#ifndef PROPERTIESMANAGER_HPP_
#define PROPERTIESMANAGER_HPP_

#include <QObject>

class PropertiesManager : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(bool showTaskTime READ showTaskTime WRITE setShowTaskTime NOTIFY showTaskTimeChanged);
    Q_PROPERTY(bool advancedMode READ advancedMode WRITE setAdvancedMode NOTIFY advancedModeChanged);
    Q_PROPERTY(bool updatingTasks READ updatingTasks NOTIFY updatingTasksChanged);
    Q_PROPERTY(bool networkIssues READ networkIssues NOTIFY networkIssuesChanged);

    static PropertiesManager *getInstance();
    PropertiesManager(QObject *parent = NULL);
    virtual ~PropertiesManager();

    bool showTaskTime();
    void setShowTaskTime(bool show);
    bool advancedMode();
    void setAdvancedMode(bool advanced);
    bool updatingTasks();
    void setUpdatingTasks(bool updating);
    bool networkIssues();
    void setNetworkIssues(bool issues);

signals:
    void showTaskTimeChanged(bool show);
    void advancedModeChanged(bool advanced);
    void updatingTasksChanged(bool updating);
    void networkIssuesChanged(bool issues);

private:
    bool _tasksUpdating;
    bool _networkIssues;

};

#endif /* PROPERTIESMANAGER_HPP_ */

#ifndef FILTERDATAMODEL_HPP_
#define FILTERDATAMODEL_HPP_

#include <QtCore/QObject>
#include <bb/cascades/GroupDataModel>
#include "NetworkManager.hpp"
#include "LoginManager.hpp"
#include "PropertiesManager.hpp"

class FilterDataModel: public bb::cascades::GroupDataModel
{
    Q_OBJECT
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged FINAL)
    Q_PROPERTY(QString filterOn READ filterOn WRITE setFilterOn NOTIFY filterOnChanged FINAL)

public:
    enum DataType
    {
        UndefinedType, Task, Hotlist, CompletedTask, Folder, Context, Goal, Location, AccountInfo
    };

    FilterDataModel(QObject *parent = 0);
    virtual ~FilterDataModel();

    virtual QVariant data(const QVariantList& indexPath);

    void setFilter(QString filter);
    void setFilterOn(QString filterOn);
    QString filter();
    QString filterOn();
    Q_INVOKABLE
    void refresh();
    void get(QString type);

    bool isFiltered(const QVariantList& indexPath);
    static const char * DataTypeStrings[];

signals:
    void filterChanged(QString filter);
    void filterOnChanged(QString filterOn);

public slots:
    void onLogOut();
    void onAddReply(int replyDataType, QVariantList dataList);
    void onEditReply(int replyDataType, QVariantList dataList);
    void onGetReply(int replyDataType, QVariantList dataList);
    void onRemoveReply(int replyDataType, QVariantList dataList);

private:
    QString _filter;
    QString _filterOn;
    bool _firstRun;
    NetworkManager *_netMan;
    LoginManager *_loginMan;
    PropertiesManager *_propMan;
    bb::cascades::DataModel *_fullDM;
    QList<QVariantMap> _originalItems;
    int _count;
};

#endif /* FILTERDATAMODEL_HPP_ */

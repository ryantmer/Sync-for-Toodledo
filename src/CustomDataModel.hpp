#ifndef CUSTOMDATAMODEL_HPP_
#define CUSTOMDATAMODEL_HPP_

#include <QObject>
#include <bb/cascades/DataModel>
#include "PropertiesManager.hpp"
#include "NetworkManager.hpp"
#include "LoginManager.hpp"

class CustomDataModel : public bb::cascades::DataModel {
    Q_OBJECT

    Q_PROPERTY(bool empty READ empty NOTIFY emptyChanged);

public:
    enum DataType {
        UndefinedType,
        Task,
        Folder,
        CompletedTask,
        Context,
        Goal,
        Location,
        AccountInfo
    };

    static const char * DataTypeStrings[];

    CustomDataModel(QObject *parent = 0, DataType dataType = UndefinedType);
    virtual ~CustomDataModel();
    //Called by the UI through QML; initiate network stuff
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void add(QVariantMap data);
    Q_INVOKABLE void edit(QVariantMap oldData, QVariantMap newData);
    Q_INVOKABLE void remove(QVariantMap data);
    Q_INVOKABLE QVariantList getInternalList();

    //Required by bb::cascades::DataModel
    Q_INVOKABLE virtual int childCount(const QVariantList &indexPath);
    Q_INVOKABLE virtual bool hasChildren(const QVariantList &indexPath);
    Q_INVOKABLE virtual QString itemType(const QVariantList &indexPath);
    Q_INVOKABLE virtual QVariant data(const QVariantList &indexPath);

private:
    void sort();
    void get();
    void clear();
    bool empty();

    QVariantList _internalDB;
    DataType _dataType;
    PropertiesManager *_propMan;
    NetworkManager *_netMan;
    LoginManager *_loginMan;

    QString getUrl;
    QString editUrl;
    QString addUrl;
    QString removeUrl;

signals:
    void emptyChanged(bool empty);

public slots:
    void onLogOut();
    void onGetReply(int replyDataType, QVariantList dataList);
    void onAddReply(int replyDataType, QVariantList dataList);
    void onEditReply(int replyDataType, QVariantList dataList);
    void onRemoveReply(int replyDataType, QVariantList dataList);
};

#endif /* CUSTOMDATAMODEL_HPP_ */

#ifndef CUSTOMDATAMODEL_HPP_
#define CUSTOMDATAMODEL_HPP_

#include <QObject>
#include <bb/cascades/DataModel>
#include "PropertiesManager.hpp"
#include "NetworkManager.hpp"
#include "LoginManager.hpp"

class CustomDataModel : public bb::cascades::DataModel {
    Q_OBJECT

public:
    enum DataType {
        UndefinedType,
        Task,
        Folder,
        CompletedTask,
        Context,
        Goal,
        Location
    };

    CustomDataModel(QObject *parent = 0);
    virtual ~CustomDataModel();
    void setDataType(DataType dataType);

    //Called by the UI through QML; initiate network stuff
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void add(QVariantMap data);
    Q_INVOKABLE void edit(QVariantMap oldData, QVariantMap newData);
    Q_INVOKABLE void remove(QVariantMap data);
    Q_INVOKABLE QVariantList getInternalList();

    static const QString getUrl;
    static const QString editUrl;
    static const QString addUrl;
    static const QString removeUrl;
    static const QString tasks;
    static const QString folders;
    static const QString contexts;
    static const QString goals;
    static const QString locations;

    //Required by bb::cascades::DataModel
    Q_INVOKABLE virtual int childCount(const QVariantList &indexPath);
    Q_INVOKABLE virtual bool hasChildren(const QVariantList &indexPath);
    Q_INVOKABLE virtual QString itemType(const QVariantList &indexPath);
    Q_INVOKABLE virtual QVariant data(const QVariantList &indexPath);

private:
    void sort();
    void populateDataModel();
    void clear();
    //Called by onReplyReceived, as required
    void addToDataModel(QVariantMap data);
    void editInDataModel(QVariantMap data);
    void removeFromDataModel(QVariantMap data);

    QVariantList _internalDB;
    DataType _dataType;
    QNetworkAccessManager *_networkAccessManager;
    PropertiesManager *_propMan;
    NetworkManager *_netMan;
    LoginManager *_loginMan;

signals:
    void toast(QString message);

public slots:
    //Signal from UI
    void onLogOut();
    //Signal from network access manager
    void onReplyReceived(QNetworkReply *reply);
};

#endif /* CUSTOMDATAMODEL_HPP_ */

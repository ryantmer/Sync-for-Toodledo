#ifndef CUSTOMDATAMODEL_HPP_
#define CUSTOMDATAMODEL_HPP_

#include <QObject>
#include <bb/cascades/DataModel>
#include "PropertiesManager.hpp"

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
    void populateDataModel();
    void clear();
    QVariantList getInternalList();

    static const QString getUrl;
    static const QString editUrl;
    static const QString addUrl;
    static const QString removeUrl;
    static const QString tasks;
    static const QString folders;

    //Required by bb::cascades::DataModel
    Q_INVOKABLE virtual int childCount(const QVariantList &indexPath);
    Q_INVOKABLE virtual bool hasChildren(const QVariantList &indexPath);
    Q_INVOKABLE virtual QString itemType(const QVariantList &indexPath);
    Q_INVOKABLE virtual QVariant data(const QVariantList &indexPath);

private:
    void sort();
    void addItem(QVariantMap data);
    void editItem(QVariantMap data);
    void removeItem(QVariantMap data);

    QVariantList _internalDB;
    DataType _dataType;
    QNetworkAccessManager *_networkAccessManager;
    PropertiesManager *_propMan;

signals:
    void toast(QString message);

public slots:
    //Signal for these four comes from UI
    void onAdd(QVariantMap data);
    void onEdit(QVariantMap oldData, QVariantMap newData);
    void onRemove(QVariantMap data);
    void onLogOut();
    //Signal from network access manager
    void onReplyReceived(QNetworkReply *reply);
};

#endif /* CUSTOMDATAMODEL_HPP_ */

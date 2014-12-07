#ifndef CUSTOMDATAMODEL_HPP_
#define CUSTOMDATAMODEL_HPP_

#include <QObject>
#include <bb/cascades/DataModel>

class CustomDataModel : public bb::cascades::DataModel {
    Q_OBJECT

public:
    enum DataType {
        Task,
        Folder
    };

    CustomDataModel(QObject *parent = 0);
    virtual ~CustomDataModel();

    void setDataType(DataType dataType);
    void clear();
    QVariant firstEntry();
    QVariantList getInternalList();

    //Required by bb::cascades::DataModel
    Q_INVOKABLE virtual int childCount(const QVariantList &indexPath);
    Q_INVOKABLE virtual bool hasChildren(const QVariantList &indexPath);
    Q_INVOKABLE virtual QString itemType(const QVariantList &indexPath);
    Q_INVOKABLE virtual QVariant data(const QVariantList &indexPath);

private:
    QVariantList _internalDB;
    DataType _dataType;
    void sort();

signals:
    void toast(QString message);

public slots:
    void onTaskEdited(QVariantMap task);
    void onTaskAdded(QVariantMap task);
    void onTaskRemoved(QVariantMap task);
    void onFolderEdited(QVariantMap folder);
    void onFolderAdded(QVariantMap folder);
    void onFolderRemoved(QVariantMap folder);
    void onLoggedOut();
};

#endif /* CUSTOMDATAMODEL_HPP_ */

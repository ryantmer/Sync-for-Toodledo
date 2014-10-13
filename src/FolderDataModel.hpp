#ifndef FOLDERDATAMODEL_HPP_
#define FOLDERDATAMODEL_HPP_

#include <QObject>
#include <bb/cascades/DataModel>

class FolderDataModel : public bb::cascades::DataModel {
    Q_OBJECT
    static const QString folderDBPath;

public:
    FolderDataModel(QObject *parent = 0);
    virtual ~FolderDataModel();

    //Required by bb::cascades::DataModel
    Q_INVOKABLE virtual int childCount(const QVariantList &indexPath);
    Q_INVOKABLE virtual bool hasChildren(const QVariantList &indexPath);
    Q_INVOKABLE virtual QString itemType(const QVariantList &indexPath);
    Q_INVOKABLE virtual QVariant data(const QVariantList &indexPath);

    QVariantList getFolderList();

private:
    QVariantList folderDB;
    void initDatabase();

signals:

public slots:
    void onFolderEdited(QVariantMap folder);
    void onFolderAdded(QVariantMap folder);
    void onFolderRemoved(QVariantMap folder);
    void onLoggedOut();
    void onAboutToQuit();
};

#endif /* FOLDERDATAMODEL_HPP_ */

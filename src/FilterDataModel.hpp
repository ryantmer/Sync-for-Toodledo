#ifndef FILTERDATAMODEL_HPP_
#define FILTERDATAMODEL_HPP_

#include <QtNetwork>
#include <QtCore/QObject>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/QListDataModel>
#include "LoginManager.hpp"
#include "PropertiesManager.hpp"

using namespace bb::cascades;

class FilterDataModel: public GroupDataModel
{
Q_OBJECT

Q_PROPERTY(bool itemsGrouped READ itemsGrouped WRITE groupItems FINAL)
Q_PROPERTY(QVariantMap filter READ filter WRITE setFilter NOTIFY filterChanged FINAL)
Q_PROPERTY(bool empty READ empty NOTIFY emptyChanged)
Q_PROPERTY(QVariantList allData READ allData FINAL)

public:
    static const QString getUrl;
    static const QString addUrl;
    static const QString editUrl;
    static const QString removeUrl;

    FilterDataModel(QObject *parent = 0);
    virtual ~FilterDataModel();

    virtual QVariant data(const QVariantList& indexPath);
    void clearByType(QString type);
    void sort();
    void get(QString type);

    bool itemsGrouped();
    QVariantMap filter();
    bool empty();
    QVariantList allData();
    void groupItems(bool grouping);
    void setFilter(QVariantMap filter);

    Q_INVOKABLE
    void refresh(QString type);
    Q_INVOKABLE
    void addItem(QString type, QVariantMap data);
    Q_INVOKABLE
    void editItem(QString type, QVariantMap data);
    Q_INVOKABLE
    void removeItem(QString type, QVariantMap data);

signals:
    void networkRequestStarted();
    void networkRequestFinished();
    void filterChanged(QVariantMap filter);
    void emptyChanged(bool empty);
    void toast(QString message);

public slots:
    void onNetworkStateChanged(bool online);
    void onAccessTokenRefreshed();
    void onFinished(QNetworkReply *reply);
    void onLogOut();

private:
    QNetworkAccessManager *_netAccMan;
    QVariantMap _filter;
    LoginManager *_loginMan;
    PropertiesManager *_propMan;
    QVariantList *_allData;
    int _count;
};

#endif /* FILTERDATAMODEL_HPP_ */

#ifndef FILTERDATAMODEL_HPP_
#define FILTERDATAMODEL_HPP_

#include <QtCore/QObject>
#include <bb/cascades/GroupDataModel>
#include "NetworkManager.hpp"

class FilterDataModel: public bb::cascades::GroupDataModel
{
    Q_OBJECT
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged FINAL)
    Q_PROPERTY(QString filterOn READ filterOn WRITE setFilterOn NOTIFY filterOnChanged FINAL)

public:
    FilterDataModel(QObject *parent = 0);
    virtual ~FilterDataModel();

    virtual QVariant data(const QVariantList& indexPath);

    void setFilter(QString filter);
    void setFilterOn(QString filterOn);
    QString filter();
    QString filterOn();

    bool isFiltered(const QVariantList& indexPath);

signals:
    void filterChanged(QString filter);
    void filterOnChanged(QString filterOn);

public slots:
    void onGetReply(int replyDataType, QVariantList dataList);

private:
    QString _filter;
    QString _filterOn;
    bool _firstRun;
    NetworkManager *_netMan;
    bb::cascades::DataModel *_fullDM;
    QList<QVariantMap> _originalItems;
    int _count;
};

#endif /* FILTERDATAMODEL_HPP_ */

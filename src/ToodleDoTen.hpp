#ifndef TOODLEDOTEN_HPP_
#define TOODLEDOTEN_HPP_

#include <QObject>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/Page>
#include <bb/cascades/WebView>
#include <bb/cascades/ScrollView>

#include "PropertiesManager.hpp"
#include "TaskDataModel.hpp"
#include "TaskRetriever.hpp"
#include "LoginManager.hpp"

using namespace bb::cascades;

class ToodleDoTen : public QObject {
    Q_OBJECT

public:
    Q_PROPERTY(TaskDataModel *dataModel READ dataModel CONSTANT);

    ToodleDoTen();
    virtual ~ToodleDoTen();
    TaskDataModel *dataModel();

    Q_INVOKABLE QDateTime unixTimeToDateTime(uint unixTime);
    Q_INVOKABLE uint dateTimeToUnixTime(QDateTime dateTime);
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void addTask(QVariantMap taskData);
    Q_INVOKABLE void editTask(QVariantMap taskData);

    void showToast(QString message);

signals:

public slots:
    void onWebViewUrlChanged(QUrl url);
    void onAccessTokenReceived(QString accessToken);

private:
    TaskDataModel *_dataModel;
    TaskRetriever *_taskRetriever;
    PropertiesManager *_propertiesManager;
    LoginManager *_loginManager;

    NavigationPane *root;

    Page *loginPage;
    ScrollView *loginScrollView;
    WebView *loginWebView;
};

#endif /* TOODLEDOTEN_HPP_ */

#ifndef SYNCFORTOODLEDO_CPP_
#define SYNCFORTOODLEDO_CPP_

#include "SyncForToodledo.hpp"

#include <bb/cascades/Container>
#include <bb/cascades/SceneCover>
#include <bb/cascades/Button>
#include <bb/cascades/TextField>
#include <bb/cascades/Label>
#include <bb/system/SystemToast>
#include <bb/system/SystemUiPosition>
#include <bb/PackageInfo>

using namespace bb::cascades;
using namespace bb::system;

SyncForToodledo::SyncForToodledo() : QObject() {
    qmlRegisterType<CustomDataModel>("DataModelUtil", 1, 0, "CustomDataModel");

    _propertiesManager = PropertiesManager::getInstance();
    _networkManager = NetworkManager::getInstance();
    _loginManager = LoginManager::getInstance();

    _taskDataModel = new CustomDataModel(this, CustomDataModel::Task);
    _folderDataModel = new CustomDataModel(this, CustomDataModel::Folder);
    _completedTaskDataModel = new CustomDataModel(this, CustomDataModel::CompletedTask);
    _contextDataModel = new CustomDataModel(this, CustomDataModel::Context);
    _locationDataModel = new CustomDataModel(this, CustomDataModel::Location);
    _goalDataModel = new CustomDataModel(this, CustomDataModel::Goal);
    _accountInfo = new CustomDataModel(this, CustomDataModel::AccountInfo);

    //Create root QML document from main.qml and expose certain variables to QML
    QmlDocument *qml = QmlDocument::create("asset:///ListHotlist.qml").parent(this);
    qml->setContextProperty("app", this);
    qml->setContextProperty("propertyManager", _propertiesManager);
    _root = qml->createRootObject<NavigationPane>();
    Application::instance()->setScene(_root);

    qml = QmlDocument::create("asset:///ActivityDialog.qml").parent(this);
    _activityDialog = qml->createRootObject<Dialog>();

    //Expose app to the main listview
    QDeclarativeEngine *engine = QmlDocument::defaultDeclarativeEngine();
    QDeclarativeContext *rootContext = engine->rootContext();
    rootContext->setContextProperty("app", this);

    //Login page, shown if required
    QmlDocument *loginQml = QmlDocument::create("asset:///Login.qml").parent(this);
    _loginPage = loginQml->createRootObject<Page>();
    _loginWebView = _loginPage->findChild<WebView*>("loginWebView");

    //Add cover QML for app
    QmlDocument *qmlCover = QmlDocument::create("asset:///Cover.qml").parent(this);
    qmlCover->setContextProperty("app", this);
    qmlCover->setContextProperty("propertyManager", _propertiesManager);
    _coverRoot = qmlCover->createRootObject<Container>();
    SceneCover *cover = SceneCover::create().content(_coverRoot);
    Application::instance()->setCover(cover);

    bool ok;
    ok = connect(_networkManager, SIGNAL(accessTokenRefreshed(QString, qlonglong)),
            this, SLOT(onAccessTokenRefreshed(QString, qlonglong)));
    Q_ASSERT(ok);
    ok = connect(_networkManager, SIGNAL(networkRequestStarted()),
            this, SLOT(onNetworkRequestStarted()));
    Q_ASSERT(ok);
    ok = connect(_networkManager, SIGNAL(networkRequestFinished()),
            this, SLOT(onNetworkRequestFinished()));
    Q_ASSERT(ok);
    ok = connect(_loginWebView, SIGNAL(urlChanged(QUrl)),
            this, SLOT(onWebViewUrlChanged(QUrl)));
    Q_ASSERT(ok);
    ok = connect(_loginManager, SIGNAL(refreshTokenExpired()),
            this, SLOT(onRefreshTokenExpired()));
    Q_ASSERT(ok);
    ok = connect(_networkManager, SIGNAL(networkStateChanged(bool)),
            this, SLOT(onNetworkStateChanged(bool)));
    Q_ASSERT(ok);
    ok = connect(_accountInfo, SIGNAL(itemsChanged(bb::cascades::DataModelChangeType::Type)),
            this, SLOT(onAccountInfoUpdated()));
    Q_ASSERT(ok);
    //YEAH TOAST
    ok = connect(_propertiesManager, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(ok);
    ok = connect(_loginManager, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(ok);
    ok = connect(_networkManager, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(ok);
    ok = connect(_taskDataModel, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(ok);
    ok = connect(_folderDataModel, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(ok);
    ok = connect(_completedTaskDataModel, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(ok);
    ok = connect(_contextDataModel, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(ok);
    ok = connect(_goalDataModel, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(ok);
    ok = connect(_locationDataModel, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(ok);
    ok = connect(_accountInfo, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(ok);

    //Logout signal
    ok = connect(this, SIGNAL(loggedOut()),
            _loginManager, SLOT(onLogOut()));
    Q_ASSERT(ok);
    ok = connect(this, SIGNAL(loggedOut()),
            _propertiesManager, SLOT(onLogOut()));
    Q_ASSERT(ok);
    ok = connect(this, SIGNAL(loggedOut()),
            _taskDataModel, SLOT(onLogOut()));
    Q_ASSERT(ok);
    ok = connect(this, SIGNAL(loggedOut()),
            _folderDataModel, SLOT(onLogOut()));
    Q_ASSERT(ok);
    ok = connect(this, SIGNAL(loggedOut()),
            _completedTaskDataModel, SLOT(onLogOut()));
    Q_ASSERT(ok);
    ok = connect(this, SIGNAL(loggedOut()),
            _contextDataModel, SLOT(onLogOut()));
    Q_ASSERT(ok);
    ok = connect(this, SIGNAL(loggedOut()),
            _goalDataModel, SLOT(onLogOut()));
    Q_ASSERT(ok);
    ok = connect(this, SIGNAL(loggedOut()),
            _locationDataModel, SLOT(onLogOut()));
    Q_ASSERT(ok);
    ok = connect(this, SIGNAL(loggedOut()),
            _accountInfo, SLOT(onLogOut()));
    Q_ASSERT(ok);

    Q_UNUSED(ok);
}

SyncForToodledo::~SyncForToodledo() {};

CustomDataModel *SyncForToodledo::taskDataModel() {
    return _taskDataModel;
}

CustomDataModel *SyncForToodledo::folderDataModel() {
    return _folderDataModel;
}

CustomDataModel *SyncForToodledo::completedTaskDataModel() {
    return _completedTaskDataModel;
}

CustomDataModel *SyncForToodledo::contextDataModel() {
    return _contextDataModel;
}

CustomDataModel *SyncForToodledo::goalDataModel() {
    return _goalDataModel;
}

CustomDataModel *SyncForToodledo::locationDataModel() {
    return _locationDataModel;
}

void SyncForToodledo::showToast(QString message) {
    SystemToast *toast = new SystemToast(this);
    toast->setBody(message);
    toast->setPosition(SystemUiPosition::MiddleCenter);
    toast->show();
}

/*
 * Q_INVOKABLE functions begin
 */
QDateTime SyncForToodledo::unixTimeToDateTime(uint unixTime) {
    return QDateTime::fromTime_t(unixTime);
}

uint SyncForToodledo::dateTimeToUnixTime(QDateTime dateTime) {
    return dateTime.toTime_t();
}

QDateTime SyncForToodledo::unixTimeToDateTimeNoOffset(uint unixTime) {
    //Calculate offset in timestamp, and remove it
    QDateTime dt1 = QDateTime::currentDateTime();
    QDateTime dt2 = dt1.toUTC();
    dt1.setTimeSpec(Qt::UTC);
    int offset = dt2.secsTo(dt1);

    return QDateTime::fromTime_t(unixTime - offset);
}

uint SyncForToodledo::dateTimeToUnixTimeNoOffset(QDateTime dateTime) {
    QDateTime dt1 = QDateTime::currentDateTime();
    QDateTime dt2 = dt1.toUTC();
    dt1.setTimeSpec(Qt::UTC);
    int offset = dt2.secsTo(dt1);

    return dateTime.toTime_t() + offset;
}

uint SyncForToodledo::getLengthValue(QDateTime dateTime) {
    return dateTime.time().hour() * 60 + dateTime.time().minute();
}

QString SyncForToodledo::getVersionNumber() {
    bb::PackageInfo pi;
    return pi.version();
}

void SyncForToodledo::getLocation() {
    QGeoPositionInfoSource *src = QGeoPositionInfoSource::createDefaultSource(this);
    src->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);

    bool ok = connect(src, SIGNAL(positionUpdated(const QGeoPositionInfo &)),
            this, SLOT(onPositionUpdated(const QGeoPositionInfo &)));
    if (ok) {
        src->requestUpdate();
    } else {
        qDebug() << Q_FUNC_INFO << "Couldn't connect to location signal";
    }
}

void SyncForToodledo::logout() {
    emit loggedOut();
}
/*
 * Q_INVOKABLE functions end
 */

/*
 * Slots begin
 */
void SyncForToodledo::onPositionUpdated(const QGeoPositionInfo &pos) {
    double lat = pos.coordinate().latitude();
    double lon = pos.coordinate().longitude();
    qDebug() << Q_FUNC_INFO << "Got location:" << lat << lon;

    Button *button = _root->findChild<Button*>("currentLocationButton");
    button->setEnabled(true);
    button->setText("Use Current Location");

    TextField *field = _root->findChild<TextField*>("latField");
    field->setText(QString::number(lat));
    field = _root->findChild<TextField*>("lonField");
    field->setText(QString::number(lon));
}

void SyncForToodledo::onNetworkStateChanged(bool connected) {
    if (connected) {
        qDebug() << Q_FUNC_INFO << "Network connection established";
        _accountInfo->refresh();
    } else {
        qWarning() << Q_FUNC_INFO << "Network connection lost";
        showToast("Network connection lost");
    }
}

void SyncForToodledo::onWebViewUrlChanged(QUrl url) {
    if (url.hasQueryItem("code") && url.hasQueryItem("state")) {
        if (url.queryItemValue("state") == _loginManager->getState()) {
            //Get authCode from webview's URL
            QString authCode = url.queryItemValue("code");
            //Start access token request
            _loginManager->refreshRefreshToken(authCode);
            //Remove webview from nav pane
            _root->pop();
        } else {
            qWarning() << Q_FUNC_INFO << "State didn't match";
        }
    }
}

void SyncForToodledo::onRefreshTokenExpired() {
    //emitted by LoginManager when refresh token is no longer valid (30-day expiry)
    //when this occurs, user has to log in again
    showToast("Please log in!");
    _root->push(_loginPage);
    _loginWebView->setUrl(_loginManager->getAuthorizeUrl().toString());
}

void SyncForToodledo::onAccessTokenRefreshed(QString newToken, qlonglong expiresIn) {
    //access token is automatically refreshed when it expires using refresh token
    //When a new access token is received, refresh account info
    Q_UNUSED(newToken);
    Q_UNUSED(expiresIn);
    _accountInfo->refresh();
}

void SyncForToodledo::onNetworkRequestStarted(QString message) {
    Label *activityText = _activityDialog->findChild<Label*>("activityText");
    activityText->setText(message);
    _activityDialog->open();
}

void SyncForToodledo::onNetworkRequestFinished() {
    _activityDialog->close();
}

void SyncForToodledo::onAccountInfoUpdated() {
    //When logout is called, first() ends up getting called on an empty list,
    //so avoid that here
    if (_accountInfo->getInternalList().length() <= 0) {
        return;
    }
    //Go through new account info and update things as required
    QVariantMap newInfo = _accountInfo->getInternalList().first().value<QVariantMap>();
    QVariantMap oldInfo = _propertiesManager->accountInfo;

    int old_lastedit_task = oldInfo.value("lastedit_task").toInt(NULL);
    int old_lastdelete_task = oldInfo.value("lastdelete_task").toInt(NULL);
    int old_lastedit_folder = oldInfo.value("lastedit_folder").toInt(NULL);
    int old_lastedit_context = oldInfo.value("lastedit_context").toInt(NULL);
    int old_lastedit_goal = oldInfo.value("lastedit_goal").toInt(NULL);
    int old_lastedit_location = oldInfo.value("lastedit_location").toInt(NULL);

    if (old_lastedit_task < newInfo.value("lastedit_task").toInt(NULL) ||
            old_lastdelete_task < newInfo.value("lastdelete_task").toInt(NULL) ||
            old_lastedit_task == 0 || old_lastdelete_task == 0) {
        qDebug() << Q_FUNC_INFO << "Refreshing Tasks";
        _taskDataModel->refresh();
        _completedTaskDataModel->refresh();
    } else {
        qDebug() << Q_FUNC_INFO << "No changes to tasks on server since last update";
    }
    if (old_lastedit_folder < newInfo.value("lastedit_folder").toInt(NULL) ||
            old_lastedit_folder == 0) {
        qDebug() << Q_FUNC_INFO << "Refreshing Folders";
        _folderDataModel->refresh();
    } else {
        qDebug() << Q_FUNC_INFO << "No changes to folders on server since last update";
    }
    if (old_lastedit_context < newInfo.value("lastedit_context").toInt(NULL) ||
            old_lastedit_context == 0) {
        qDebug() << Q_FUNC_INFO << "Refreshing Contexts";
        _contextDataModel->refresh();
    } else {
        qDebug() << Q_FUNC_INFO << "No changes to contexts on server since last update";
    }
    if (old_lastedit_goal < newInfo.value("lastedit_goal").toInt(NULL) ||
            old_lastedit_goal == 0) {
        qDebug() << Q_FUNC_INFO << "Refreshing Goals";
        _goalDataModel->refresh();
    } else {
        qDebug() << Q_FUNC_INFO << "No changes to goals on server since last update";
    }
    if (old_lastedit_location < newInfo.value("lastedit_location").toInt(NULL) ||
            old_lastedit_location == 0) {
        qDebug() << Q_FUNC_INFO << "Refreshing Locations";
        _locationDataModel->refresh();
    } else {
        qDebug() << Q_FUNC_INFO << "No changes to locations on server since last update";
    }

    _propertiesManager->accountInfo = newInfo;
}

void SyncForToodledo::onToast(QString message) {
    showToast(message);
}
/*
 * Slots end
 */

#endif /* SYNCFORTOODLEDO_CPP_ */

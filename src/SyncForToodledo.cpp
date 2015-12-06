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

SyncForToodledo::SyncForToodledo() :
        QObject(), _propertiesManager(PropertiesManager::getInstance()), _loginManager(
                LoginManager::getInstance()), _data(new FilterDataModel(this))
{
    qmlRegisterType < FilterDataModel > ("DataModelUtil", 1, 0, "FilterDataModel");

    //Create root QML document from main.qml and expose certain variables to QML
    QmlDocument *qml = QmlDocument::create("asset:///MainTabbedPane.qml").parent(this);
    qml->setContextProperty("app", this);
    qml->setContextProperty("propertyManager", _propertiesManager);
    _root = qml->createRootObject<TabbedPane>();
    Application::instance()->setScene(_root);

    qml = QmlDocument::create("asset:///ActivityDialog.qml").parent(this);
    _activityDialog = qml->createRootObject<Dialog>();

    //Expose app to the main listview
    QDeclarativeEngine *engine = QmlDocument::defaultDeclarativeEngine();
    QDeclarativeContext *rootContext = engine->rootContext();
    rootContext->setContextProperty("app", this);

    //Login page, shown if required
    QmlDocument *loginQml = QmlDocument::create("asset:///Login.qml").parent(this);
    _loginSheet = loginQml->createRootObject<Sheet>();
    _loginWebView = _loginSheet->findChild<WebView*>("loginWebView");

    //Add cover QML for app
    QmlDocument *qmlCover = QmlDocument::create("asset:///Cover.qml").parent(this);
    qmlCover->setContextProperty("app", this);
    qmlCover->setContextProperty("propertyManager", _propertiesManager);
    _coverRoot = qmlCover->createRootObject<Container>();
    SceneCover *cover = SceneCover::create().content(_coverRoot);
    Application::instance()->setCover(cover);

    bool ok;
    ok = connect(_loginWebView, SIGNAL(urlChanged(QUrl)), this, SLOT(onWebViewUrlChanged(QUrl)));
    Q_ASSERT(ok);
    ok = connect(_loginManager, SIGNAL(refreshTokenExpired()), this, SLOT(onRefreshTokenExpired()));
    Q_ASSERT(ok);
    ok = connect(this, SIGNAL(loggedOut()), _loginManager, SLOT(onLogOut()));
    Q_ASSERT(ok);
    ok = connect(this, SIGNAL(loggedOut()), _propertiesManager, SLOT(onLogOut()));
    Q_ASSERT(ok);
    ok = connect(_loginManager, SIGNAL(networkRequestStarted()), this, SLOT(onNetworkRequestStarted()));
    Q_ASSERT(ok);
    ok = connect(_loginManager, SIGNAL(networkRequestFinished()), this, SLOT(onNetworkRequestFinished()));
    Q_ASSERT(ok);
    ok = connect(_data, SIGNAL(networkRequestStarted()), this, SLOT(onNetworkRequestStarted()));
    Q_ASSERT(ok);
    ok = connect(_data, SIGNAL(networkRequestFinished()), this, SLOT(onNetworkRequestFinished()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

SyncForToodledo::~SyncForToodledo()
{
}

FilterDataModel *SyncForToodledo::data()
{
    return _data;
}

void SyncForToodledo::showToast(QString message)
{
    SystemToast *toast = new SystemToast(this);
    toast->setBody(message);
    toast->setPosition(SystemUiPosition::MiddleCenter);
    toast->show();
}

/*
 * Q_INVOKABLE functions begin
 */
QDateTime SyncForToodledo::unixTimeToDateTime(uint unixTime)
{
    return QDateTime::fromTime_t(unixTime);
}

uint SyncForToodledo::dateTimeToUnixTime(QDateTime dateTime)
{
    return dateTime.toTime_t();
}

QDateTime SyncForToodledo::unixTimeToDateTimeNoOffset(uint unixTime)
{
    //Calculate offset in timestamp, and remove it
    QDateTime dt1 = QDateTime::currentDateTime();
    QDateTime dt2 = dt1.toUTC();
    dt1.setTimeSpec(Qt::UTC);
    int offset = dt2.secsTo(dt1);

    return QDateTime::fromTime_t(unixTime - offset);
}

uint SyncForToodledo::dateTimeToUnixTimeNoOffset(QDateTime dateTime)
{
    QDateTime dt1 = QDateTime::currentDateTime();
    QDateTime dt2 = dt1.toUTC();
    dt1.setTimeSpec(Qt::UTC);
    int offset = dt2.secsTo(dt1);

    return dateTime.toTime_t() + offset;
}

uint SyncForToodledo::getLengthValue(QDateTime dateTime)
{
    return dateTime.time().hour() * 60 + dateTime.time().minute();
}

QString SyncForToodledo::getVersionNumber()
{
    bb::PackageInfo pi;
    return pi.version();
}

void SyncForToodledo::getLocation()
{
    QGeoPositionInfoSource *src = QGeoPositionInfoSource::createDefaultSource(this);
    src->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);

    bool ok = connect(src, SIGNAL(positionUpdated(const QGeoPositionInfo &)), this,
            SLOT(onPositionUpdated(const QGeoPositionInfo &)));
    if (ok) {
        src->requestUpdate();
    } else {
        qDebug() << Q_FUNC_INFO << "Couldn't connect to location signal";
    }
}

void SyncForToodledo::logout()
{
    emit loggedOut();
}
/*
 * Q_INVOKABLE functions end
 */

/*
 * Slots begin
 */
void SyncForToodledo::onPositionUpdated(const QGeoPositionInfo &pos)
{
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

void SyncForToodledo::onNetworkStateChanged(bool connected)
{
    if (connected) {
        qDebug() << Q_FUNC_INFO << "Network connection established";
        _data->refresh("account");
    } else {
        qWarning() << Q_FUNC_INFO << "Network connection lost";
        showToast("Network connection lost");
    }
}

void SyncForToodledo::onWebViewUrlChanged(QUrl url)
{
    if (url.hasQueryItem("code") && url.hasQueryItem("state")) {
        if (url.queryItemValue("state") == _loginManager->getState()) {
            //Get authCode from webview's URL
            QString authCode = url.queryItemValue("code");
            //Start access token request
            _loginManager->refreshRefreshToken(authCode);
            _loginSheet->close();
        } else {
            qWarning() << Q_FUNC_INFO << "State didn't match";
        }
    }
}

void SyncForToodledo::onRefreshTokenExpired()
{
    //emitted by LoginManager when refresh token is no longer valid (30-day expiry)
    //when this occurs, user has to log in again
    showToast("Please log in!");
    _loginSheet->open();
    _loginWebView->setUrl(_loginManager->getAuthorizeUrl().toString());
}

void SyncForToodledo::onNetworkRequestStarted(QString message)
{
    Label *activityText = _activityDialog->findChild<Label*>("activityText");
    activityText->setText(message);
    _activityDialog->open();
}

void SyncForToodledo::onNetworkRequestFinished()
{
    _activityDialog->close();
}

void SyncForToodledo::onToast(QString message)
{
    showToast(message);
}
/*
 * Slots end
 */

#endif /* SYNCFORTOODLEDO_CPP_ */

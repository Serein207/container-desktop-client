#include "NetworkClient.h"
#include "Api2.hpp"
#include "Infrastructure/Deserializer/Deserializer.hpp"
#include "Infrastructure/Utility/Result.hpp"
#include "Model/ContainerBlock.h"
#include "Model/RrdData.h"
#include "Model/Snapshot.h"
#include "Model/Status.h"
#include "Model/VzTemp.h"
#include "TcpSockify.h"
#include <QNetworkProxy>
#include <functional>
#include <optional>
#include <qdatetime.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qlist.h>
#include <qurl.h>
#include <qurlquery.h>
#include <utility>

static auto API_GATEWAY = QStringLiteral("http://192.168.0.195");

ContainerDesktop::NetworkClient::NetworkClient() : tcpSockify(new TcpSockify(this)) {
    // manager.setProxy(QNetworkProxy::NoProxy);
}

ContainerDesktop::NetworkClient::~NetworkClient() = default;

ContainerDesktop::NetworkClient* ContainerDesktop::NetworkClient::getInstance() {
    static NetworkClient instance;
    return &instance;
}

QNetworkReply* ContainerDesktop::NetworkClient::createReply(ContainerDesktop::Method verb,
                                                            const QNetworkRequest& requestInfo,
                                                            const QByteArray& body) {
    switch (verb) {
    case Method::POST: {
        return manager.post(requestInfo, body);
    }
    case Method::PUT: {
        return manager.put(requestInfo, body);
    }
    case Method::DELETE: {
        assert(body.isEmpty());
        return manager.deleteResource(requestInfo);
    }
    case Method::GET: {
        assert(body.isEmpty());
        return manager.get(requestInfo);
    }
    case Method::UNSPECIFIED: {
        return manager.sendCustomRequest(requestInfo, "INVALID", body);
    }
    }
    return nullptr;
}

void ContainerDesktop::NetworkClient::setCookieJar(QNetworkCookieJar* cookieJar) {
    manager.setCookieJar(cookieJar);
    auto cookies = cookieJar->cookiesForUrl(API_GATEWAY);
    if (!cookies.empty())
        this->ticket = cookies.first().value();
}

void ContainerDesktop::NetworkClient::login(const QString& username, const QString& password,
                                            std::function<void(Result<User>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/access/ticket");
    auto data =
        QJsonDocument(QJsonObject{{"username", username + u"@pve"_qs}, {"password", password}});
    request<Api2>(Method::POST, url, data,
                  [callback = std::move(callback), this](Result<QJsonObject> result) {
                      if (result.isOk()) {
                          auto data = result.unwrap();
                          this->tokenBytes = data["CSRFPreventionToken"].toString().toUtf8();
                          this->ticket = data["ticket"].toString().toUtf8();
                          manager.cookieJar()->insertCookie(
                              QNetworkCookie("PVEAuthCookie", ticket));
                      }
                      callback(result.andThen(Deserializer<User>::from));
                  });
}

void ContainerDesktop::NetworkClient::registerUser(
    const QString& username, const QString& password, const std::optional<QString>& email,
    std::function<void(Result<QJsonObject>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/access/users");
    QJsonValue emailValue = email.has_value() ? QJsonValue(email.value()) : QJsonValue::Null;
    auto data = QJsonDocument(QJsonObject{
        {"userid", username + u"@pve"_qs}, {"password", password}, {"email", emailValue}});
    request<Api2>(
        Method::POST, url, data,
        [callback = std::move(callback)](Result<QJsonObject> result) { callback(result); });
}

void ContainerDesktop::NetworkClient::modifyPassword(
    const QString& username, const QString& password,
    std::function<void(Result<QJsonObject>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/access/password");
    auto data =
        QJsonDocument(QJsonObject{{"userid", username + u"@pve"_qs}, {"password", password}});
    request<Api2>(
        Method::PUT, url, data,
        [callback = std::move(callback)](Result<QJsonObject> result) { callback(result); });
}

void ContainerDesktop::NetworkClient::getProfile(std::function<void(Result<Profile>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/access/users");
    auto data = QJsonDocument();
    request<Api2>(Method::GET, url, data,
                  [callback = std::move(callback)](Result<QJsonObject> result) {
                      callback(result.andThen(Deserializer<Profile>::from));
                  });
}

void ContainerDesktop::NetworkClient::modifyProfile(
    const QString& username, const QString& newEmail, const QString& newFirstName,
    const QString& newLastName, std::function<void(Result<QJsonObject>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/access/users/" + username + u"@pve"_qs);
    auto data = QJsonDocument(
        QJsonObject{{"email", newEmail}, {"firstname", newFirstName}, {"lastname", newLastName}});
    request<Api2>(
        Method::PUT, url, data,
        [callback = std::move(callback)](Result<QJsonObject> result) { callback(result); });
}

void ContainerDesktop::NetworkClient::getClusterStatusInfo(
    std::function<void(Result<QJsonArray>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/cluster/resources");
    auto data = QJsonDocument();
    request<Api2>(
        Method::GET, url, data,
        [callback = std::move(callback)](Result<QJsonArray> result) { callback(result); });
}

void ContainerDesktop::NetworkClient::getAllContainerInfo(
    const QString& node, std::function<void(Result<QList<ContainerBlock>>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/nodes/" + node + "/lxc");
    auto data = QJsonDocument();
    request<Api2>(Method::GET, url, data,
                  [callback = std::move(callback)](Result<QJsonArray> result) {
                      callback(result.andThen(Deserializer<QList<ContainerBlock>>::from));
                  });
}

void ContainerDesktop::NetworkClient::connectVnc(const QString& ip, quint16 port) {
    tcpSockify->connectToServer(
        QStringLiteral("ws://192.168.0.195/vnc/%1/%2/connect").arg(ip).arg(port));
}

void ContainerDesktop::NetworkClient::disconnectVnc() {
    tcpSockify->disconnectFromServer();
}

void ContainerDesktop::NetworkClient::startContainer(
    const QString& node, const QString& vmId, std::function<void(Result<QJsonObject>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/nodes/" + node + "/lxc/" + vmId + "/status/start");
    auto data = QJsonDocument();
    request<Api2>(
        Method::POST, url, data,
        [callback = std::move(callback)](Result<QJsonObject> result) { callback(result); });
}

void ContainerDesktop::NetworkClient::rebootContainer(
    const QString& node, const QString& vmId, std::function<void(Result<QJsonObject>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/nodes/" + node + "/lxc/" + vmId + "/status/reboot");
    auto data = QJsonDocument();
    request<Api2>(
        Method::POST, url, data,
        [callback = std::move(callback)](Result<QJsonObject> result) { callback(result); });
}

void ContainerDesktop::NetworkClient::shutdownContainer(
    const QString& node, const QString& vmId, std::function<void(Result<QJsonObject>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/nodes/" + node + "/lxc/" + vmId + "/status/shutdown");
    auto data = QJsonDocument();
    request<Api2>(
        Method::POST, url, data,
        [callback = std::move(callback)](Result<QJsonObject> result) { callback(result); });
}

void ContainerDesktop::NetworkClient::stopContainer(
    const QString& node, const QString& vmId, std::function<void(Result<QJsonObject>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/nodes/" + node + "/lxc/" + vmId + "/status/stop");
    auto data = QJsonDocument();
    request<Api2>(
        Method::POST, url, data,
        [callback = std::move(callback)](Result<QJsonObject> result) { callback(result); });
}

void ContainerDesktop::NetworkClient::suspendContainer(
    const QString& node, const QString& vmId, std::function<void(Result<QJsonObject>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/nodes/" + node + "/lxc/" + vmId + "/status/suspend");
    auto data = QJsonDocument();
    request<Api2>(
        Method::POST, url, data,
        [callback = std::move(callback)](Result<QJsonObject> result) { callback(result); });
}

void ContainerDesktop::NetworkClient::resumeContainer(
    const QString& node, const QString& vmId, std::function<void(Result<QJsonObject>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/nodes/" + node + "/lxc/" + vmId + "/status/resume");
    auto data = QJsonDocument();
    request<Api2>(
        Method::POST, url, data,
        [callback = std::move(callback)](Result<QJsonObject> result) { callback(result); });
}

void ContainerDesktop::NetworkClient::getRrdData(
    const QString& node, const QString& vmId, const QString& timeFrame,
    std::function<void(Result<QList<RrdData>>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/nodes/" + node + "/lxc/" + vmId +
                    "/rrddata?timeframe=" + timeFrame);
    auto data = QJsonDocument();
    request<Api2>(Method::GET, url, data,
                  [callback = std::move(callback)](Result<QJsonArray> result) {
                      callback(result.andThen(Deserializer<QList<RrdData>>::from));
                  });
}

void ContainerDesktop::NetworkClient::getVzTemplates(
    const QString& node, std::function<void(Result<QList<VzTemp>>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/nodes/" + node + "/vztmpl");
    auto data = QJsonDocument();
    request<Api2>(Method::GET, url, data,
                  [callback = std::move(callback)](Result<QJsonArray> result) {
                      callback(result.andThen(Deserializer<QList<VzTemp>>::from));
                  });
}

void ContainerDesktop::NetworkClient::getConfig(const QString& node, const QString& vmId,
                                                std::function<void(Result<Config>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/nodes/" + node + "/lxc/" + vmId + "/config");
    auto data = QJsonDocument();
    request<Api2>(Method::GET, url, data,
                  [callback = std::move(callback)](Result<QJsonObject> result) {
                      callback(result.andThen(Deserializer<Config>::from));
                  });
}

void ContainerDesktop::NetworkClient::createContainer(
    const QString& node, const QString& osTemplate, const QString& userPassword,
    const QString& vncPassword, std::function<void(Result<QJsonObject>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/nodes/" + node + "/lxc");
    auto data = QJsonDocument(QJsonObject{
        {"ostemplate", osTemplate}, {"userpassword", userPassword}, {"vncpassword", vncPassword}});
    request<Api2>(
        Method::POST, url, data,
        [callback = std::move(callback)](Result<QJsonObject> result) { callback(result); });
}

void ContainerDesktop::NetworkClient::getNodeStatus(const QString& node,
                                                    std::function<void(Result<Status>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/nodes/" + node + "/status");
    auto data = QJsonDocument();
    request<Api2>(Method::GET, url, data,
                  [callback = std::move(callback)](Result<QJsonObject> result) {
                      callback(result.andThen(Deserializer<Status>::from));
                  });
}

void ContainerDesktop::NetworkClient::getSnapshots(
    const QString& node, const QString& vmId,
    std::function<void(Result<QList<Snapshot>>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/nodes/" + node + "/lxc/" + vmId + "/snapshot");
    auto data = QJsonDocument();
    request<Api2>(Method::GET, url, data,
                  [callback = std::move(callback)](Result<QJsonArray> result) {
                      callback(result.andThen(Deserializer<QList<Snapshot>>::from));
                  });
}

void ContainerDesktop::NetworkClient::createSnapshot(
    const QString& node, const QString& vmId, const QString& snapName,
    std::function<void(Result<QList<Snapshot>>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/nodes/" + node + "/lxc/" + vmId + "/snapshot");
    auto data = QJsonDocument(QJsonObject{
        {"snapname", snapName},
        {"description", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz 备份")}});
    request<Api2>(Method::POST, url, data,
                  [callback = std::move(callback)](Result<QJsonArray> result) {
                      callback(result.andThen(Deserializer<QList<Snapshot>>::from));
                  });
}

void ContainerDesktop::NetworkClient::deleteSnapshot(
    const QString& node, const QString& vmId, const QString& snapName,
    std::function<void(Result<QList<Snapshot>>)> callback) {
    auto url =
        QUrl(API_GATEWAY + "/api2/json/nodes/" + node + "/lxc/" + vmId + "/snapshot/" + snapName);
    auto data = QJsonDocument();
    request<Api2>(Method::DELETE, url, data,
                  [callback = std::move(callback)](Result<QJsonArray> result) {
                      callback(result.andThen(Deserializer<QList<Snapshot>>::from));
                  });
}

void ContainerDesktop::NetworkClient::rollbackSnapshot(
    const QString& node, const QString& vmId, const QString& snapName,
    std::function<void(Result<QList<Snapshot>>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/nodes/" + node + "/lxc/" + vmId + "/snapshot/" +
                    snapName + "/rollback");
    auto data = QJsonDocument();
    request<Api2>(Method::POST, url, data,
                  [callback = std::move(callback)](Result<QJsonArray> result) {
                      callback(result.andThen(Deserializer<QList<Snapshot>>::from));
                  });
}

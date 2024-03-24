#include "NetworkClient.h"
#include "Api2.hpp"
#include "Infrastructure/Deserializer/Deserializer.hpp"
#include "Infrastructure/Utility/Result.hpp"
#include "Model/ContainerBlock.h"
#include <QNetworkProxy>
#include <functional>
#include <qjsonarray.h>

static auto API_GATEWAY = QStringLiteral("https://container-desk.top");

ContainerDesktop::NetworkClient::NetworkClient() {
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
    const QString& username, const QString& password,
    std::function<void(Result<QJsonObject>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/access/users");
    auto data =
        QJsonDocument(QJsonObject{{"userid", username + u"@pve"_qs}, {"password", password}});
    request<Api2>(
        Method::POST, url, data,
        [callback = std::move(callback)](Result<QJsonObject> result) { callback(result); });
}

void ContainerDesktop::NetworkClient::getClusterStatusInfo(
    std::function<void(Result<QJsonArray>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/cluster/status");
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
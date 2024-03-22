#include "NetworkClient.h"
#include "Api2.hpp"
#include "Infrastructure/Deserializer/Deserializer.hpp"
#include <QNetworkProxy>
#include <qnetworkcookie.h>

static auto API_GATEWAY = QStringLiteral("https://container-desk.top");

ContainerDesktop::NetworkClient::NetworkClient() {
    // manager.setProxy(QNetworkProxy::NoProxy);
}

ContainerDesktop::NetworkClient::~NetworkClient() = default;

ContainerDesktop::NetworkClient*
    ContainerDesktop::NetworkClient::getInstance() {
    static NetworkClient instance;
    return &instance;
}

QNetworkReply* ContainerDesktop::NetworkClient::createReply(
    ContainerDesktop::Method verb, const QNetworkRequest& requestInfo,
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

void ContainerDesktop::NetworkClient::setCookieJar(
    QNetworkCookieJar* cookieJar) {
    manager.setCookieJar(cookieJar);
}

void ContainerDesktop::NetworkClient::login(
    QStringView username, QStringView password,
    std::function<void(Result<User>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/access/ticket");
    auto data = QJsonDocument(
        QJsonObject{{"username", username.toString() + u"@pve"_qs},
                    {"password", password.toString()}});
    request<Api2>(
        Method::POST, url, data,
        [callback = std::move(callback), this](Result<QJsonObject> result) {
            if (result.isOk()) {
                auto data = result.unwrap()["data"].toObject();
                this->tokenBytes =
                    data["CSRFPreventionToken"].toString().toUtf8();
                manager.cookieJar()->insertCookie(QNetworkCookie(
                    "PVEAuthCookie", data["ticket"].toString().toUtf8()));
            }
            callback(result.andThen(Deserializer<User>::from));
        });
}

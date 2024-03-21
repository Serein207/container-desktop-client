#include "NetworkClient.h"
#include "Api2.hpp"
#include <QNetworkProxy>

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
    const QByteArray& verb, const QNetworkRequest& requestInfo,
    const QByteArray& body) {
    if (verb.compare("POST", Qt::CaseInsensitive) == 0) {
        return manager.post(requestInfo, body);
    } else if (verb.compare("PUT", Qt::CaseInsensitive) == 0) {
        return manager.put(requestInfo, body);
    } else if (verb.compare("DELETE", Qt::CaseInsensitive) == 0) {
        assert(body.isEmpty());
        return manager.deleteResource(requestInfo);
    } else if (verb.compare("GET", Qt::CaseInsensitive) == 0) {
        assert(body.isEmpty());
        return manager.get(requestInfo);
    } else {
        return manager.sendCustomRequest(requestInfo, verb, body);
    }
}

void ContainerDesktop::NetworkClient::setCookieJar(
    QNetworkCookieJar* cookieJar) {
    manager.setCookieJar(cookieJar);
}

void ContainerDesktop::NetworkClient::login(
    QStringView username, QStringView password,
    std::function<void(Result<QJsonObject>)> callback) {
    auto url = QUrl(API_GATEWAY + "/api2/json/access/ticket");
    auto data = QJsonDocument(QJsonObject{{"username", username.toString()},
                                          {"password", password.toString()}});
    request<Api2>(
        "POST", url, data,
        [callback = std::move(callback), this](Result<QJsonObject> result) {
            if (result.isOk()) {
                this->tokenBytes =
                    result.unwrap()["CSRFPreventionToken"].toString().toUtf8();
            }
            callback(result);
        });
}
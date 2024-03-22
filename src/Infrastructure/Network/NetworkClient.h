#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include "Infrastructure/Utility/Result.hpp"
#include "Model/User.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringView>
#include <QThread>
#include <QUrl>
#include <functional>

namespace ContainerDesktop {
inline namespace ContainerDesktopDetails {
constexpr int SUCCESS_CODES[] = {200, 201, 302, 400, 502, 800, 801, 802, 803};
enum class Method { UNSPECIFIED, GET, POST, PUT, DELETE };
} // namespace ContainerDesktopDetails

class NetworkClient : public QObject {
    Q_OBJECT
public:
    NetworkClient();
    ~NetworkClient();
    static NetworkClient* getInstance();
    template <typename TEncryption, typename TCallback>
    void request(ContainerDesktopDetails::Method verb, const QUrl& url,
                 const QJsonDocument& data, TCallback callback) {
        auto requestResult =
            TEncryption::makeRequest(verb, url, manager.cookieJar(), data);
        auto [requestInfo, body] = std::move(requestResult);
        if (!tokenBytes.isEmpty())
            requestInfo.setRawHeader("CSRFPreventionToken", tokenBytes);
        QNetworkReply* reply;
        if (manager.thread() == QThread::currentThread()) {
            reply = createReply(verb, requestInfo, body);
        } else {
            QMetaObject::invokeMethod(
                &manager,
                [this, &verb, requestInfo = std::move(requestInfo),
                 body = std::move(body)] {
                    return createReply(verb, requestInfo, body);
                },
                Qt::BlockingQueuedConnection, &reply);
        }
        connect(reply, &QNetworkReply::finished, [=] {
            auto content = reply->readAll();
            auto networkError = reply->error();

            qDebug() << content.toStdString().c_str();

            if (networkError != QNetworkReply::NoError) {
                callback(Result<QJsonObject>(
                    ErrorInfo{ErrorKind::NetworkError, reply->errorString()}));
                reply->deleteLater();
                return;
            }
            Result<QByteArray> decryptedResult = TEncryption::decryptResponse(
                reply->rawHeader("Content-Type"), std::move(content));
            if (decryptedResult.isErr()) {
                callback(Result<QJsonObject>(
                    std::move(decryptedResult).unwrapErr()));
                reply->deleteLater();
                return;
            }
            auto jsonResult =
                QJsonDocument::fromJson(std::move(decryptedResult).unwrap());
            if (jsonResult.isNull() || jsonResult["data"].isNull()) {
                callback(Result<QJsonObject>(ErrorInfo{
                    ErrorKind::JsonParseError, "Failed to parse JSON"}));
                reply->deleteLater();
                return;
            }
            auto json = jsonResult.object();
            if (json.contains("code")) {
                int code = json["code"].toInt();
                auto found = std::find_if(
                    std::begin(ContainerDesktopDetails::SUCCESS_CODES),
                    std::end(ContainerDesktopDetails::SUCCESS_CODES),
                    [code](int x) { return x == code; });
                if (found == std::end(ContainerDesktopDetails::SUCCESS_CODES)) {
                    QString message;
                    if (json.contains("message")) {
                        message = json["message"].toString();
                    } else {
                        message =
                            QStringLiteral("API error code = %1").arg(code);
                    }
                    callback(Result<QJsonObject>(
                        ErrorInfo{ErrorKind::ApiError, std::move(message)}));
                    return;
                }
            }
            callback(Result<QJsonObject>(json["data"].toObject()));
            reply->deleteLater();
        });
    }

    void setCookieJar(QNetworkCookieJar* cookieJar);
    void login(QStringView username, QStringView password,
               std::function<void(Result<User>)> callback);

private:
    QNetworkAccessManager manager;
    QByteArray tokenBytes;
    QNetworkReply* createReply(ContainerDesktop::Method verb,
                               const QNetworkRequest& requestInfo,
                               const QByteArray& body);
};
} // namespace ContainerDesktop
#endif // NETWORKCLIENT_H

#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include "Infrastructure/Utility/Result.hpp"
#include "Model/User.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringView>
#include <QThread>
#include <QUrl>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <type_traits>

namespace ContainerDesktop {
inline namespace ContainerDesktopDetails {
constexpr int SUCCESS_CODES[] = {0, 1};
enum class Method { UNSPECIFIED, GET, POST, PUT, DELETE };
} // namespace ContainerDesktopDetails

class NetworkClient : public QObject {
    Q_OBJECT

public:
    NetworkClient();
    ~NetworkClient();
    static NetworkClient* getInstance();
    template <typename TEncryption, typename TCallback>
    void request(ContainerDesktopDetails::Method verb, const QUrl& url, const QJsonDocument& data,
                 TCallback callback) {
        auto requestResult = TEncryption::makeRequest(verb, url, manager.cookieJar(), data);
        auto [requestInfo, body] = std::move(requestResult);
        if (!tokenBytes.isEmpty())
            requestInfo.setRawHeader("CSRFPreventionToken", tokenBytes);
        QNetworkReply* reply;
        if (manager.thread() == QThread::currentThread()) {
            reply = createReply(verb, requestInfo, body);
        } else {
            QMetaObject::invokeMethod(
                &manager,
                [this, &verb, requestInfo = std::move(requestInfo), body = std::move(body)] {
                    return createReply(verb, requestInfo, body);
                },
                Qt::BlockingQueuedConnection, &reply);
        }
        connect(reply, &QNetworkReply::finished, [=] {
            auto content = reply->readAll();
            auto networkError = reply->error();

            qDebug() << content.toStdString().c_str();

            if (networkError != QNetworkReply::NoError) {
                callback(
                    Result<QJsonObject>(ErrorInfo{ErrorKind::NetworkError, reply->errorString()}));
                reply->deleteLater();
                return;
            }
            Result<QByteArray> decryptedResult =
                TEncryption::decryptResponse(reply->rawHeader("Content-Type"), std::move(content));
            if (decryptedResult.isErr()) {
                callback(Result<QJsonObject>(std::move(decryptedResult).unwrapErr()));
                reply->deleteLater();
                return;
            }
            auto jsonResult = QJsonDocument::fromJson(std::move(decryptedResult).unwrap());
            if (jsonResult.isNull()) {
                callback(Result<QJsonObject>(
                    ErrorInfo{ErrorKind::JsonParseError, "Failed to parse JSON"}));
                reply->deleteLater();
                return;
            }
            auto json = jsonResult.object();
            if (json.contains("success")) {
                int code = json["success"].toInt();
                if (code == 0) {
                    QString message;
                    if (json.contains("message")) {
                        message = json["message"].toString();
                    } else {
                        message = QStringLiteral("API error code = %1").arg(code);
                    }
                    callback(
                        Result<QJsonObject>(ErrorInfo{ErrorKind::ApiError, std::move(message)}));
                    return;
                }
            }
            if (json.contains("data")) {
                if constexpr (std::is_invocable_v<TCallback, Result<QJsonObject>>) {
                    if (json["data"].isObject())
                        callback(Result<QJsonObject>(json["data"].toObject()));
                    else if (json["data"].isNull())
                        callback(Result<QJsonObject>(QJsonObject{}));
                } else if constexpr (std::is_invocable_v<TCallback, Result<QJsonArray>>) {
                    if (json["data"].isArray())
                        callback(Result<QJsonArray>(json["data"].toArray()));
                    else if (json["data"].isNull())
                        callback(Result<QJsonArray>(
                            ErrorInfo{ErrorKind::JsonParseError, "Failed to parse JSON"}));
                }
            }
            reply->deleteLater();
        });
    }

    QByteArray getTicket() {
        return ticket;
    }

    void setCookieJar(QNetworkCookieJar* cookieJar);
    void login(const QString& username, const QString& password,
               std::function<void(Result<User>)> callback);
    void registerUser(const QString& username, const QString& password,
                      std::function<void(Result<QJsonObject>)> callback);

private:
    QNetworkAccessManager manager;
    QByteArray tokenBytes;
    QByteArray ticket;
    QNetworkReply* createReply(ContainerDesktop::Method verb, const QNetworkRequest& requestInfo,
                               const QByteArray& body);
};
} // namespace ContainerDesktop
#endif // NETWORKCLIENT_H

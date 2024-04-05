#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include "Infrastructure/Utility/Result.hpp"
#include "Model/Config.h"
#include "Model/ContainerBlock.h"
#include "Model/Profile.h"
#include "Model/RrdData.h"
#include "Model/Snapshot.h"
#include "Model/User.h"
#include "Model/VzTemp.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringView>
#include <QThread>
#include <QUrl>
#include <functional>

class TcpSockify;
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
    template <typename TEncryption, typename TCallback,
              typename QJsonType = std::conditional_t<
                  std::is_invocable_v<TCallback, Result<QJsonObject>>, QJsonObject, QJsonArray>>
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
            qDebug() << "Network error: " << networkError;
            qDebug() << content.toStdString().c_str();

            if (networkError != QNetworkReply::NoError) {
                callback(
                    Result<QJsonType>(ErrorInfo{ErrorKind::NetworkError, reply->errorString()}));
                reply->deleteLater();
                return;
            }
            Result<QByteArray> decryptedResult =
                TEncryption::decryptResponse(reply->rawHeader("Content-Type"), std::move(content));
            if (decryptedResult.isErr()) {
                callback(Result<QJsonType>(std::move(decryptedResult).unwrapErr()));
                reply->deleteLater();
                return;
            }
            auto jsonResult = QJsonDocument::fromJson(std::move(decryptedResult).unwrap());
            if (jsonResult.isNull()) {
                callback(Result<QJsonType>(
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
                    callback(Result<QJsonType>(ErrorInfo{ErrorKind::ApiError, std::move(message)}));
                    return;
                }
            }
            if (json.contains("data")) {
                if constexpr (std::is_invocable_v<TCallback, Result<QJsonObject>>) {
                    if (json["data"].isObject())
                        callback(Result<QJsonObject>(json["data"].toObject()));
                    else
                        callback(Result<QJsonObject>(QJsonObject{}));
                } else if constexpr (std::is_invocable_v<TCallback, Result<QJsonArray>>) {
                    if (json["data"].isArray())
                        callback(Result<QJsonArray>(json["data"].toArray()));
                    else
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

    // user
    void setCookieJar(QNetworkCookieJar* cookieJar);
    void login(const QString& username, const QString& password,
               std::function<void(Result<User>)> callback);
    void registerUser(const QString& username, const QString& password,
                      const std::optional<QString>& email,
                      std::function<void(Result<QJsonObject>)> callback);
    void modifyPassword(const QString& username, const QString& password,
                        std::function<void(Result<QJsonObject>)> callback);
    void getProfile(const QString& username, std::function<void(Result<Profile>)> callback);
    void modifyProfile(const QString& username, const QString& newEmail,
                       const QString& newFirstName, const QString& newLastName,
                       std::function<void(Result<QJsonObject>)> callback);

    // container
    void getClusterStatusInfo(std::function<void(Result<QJsonArray>)> callback);
    void getAllContainerInfo(const QString& node,
                             std::function<void(Result<QList<ContainerBlock>>)> callback);
    void connectVnc(const QString& ip, quint16 port);
    void disconnectVnc();
    void startContainer(const QString& node, const QString& vmId,
                        std::function<void(Result<QJsonObject>)> callback);
    void rebootContainer(const QString& node, const QString& vmId,
                         std::function<void(Result<QJsonObject>)> callback);
    void shutdownContainer(const QString& node, const QString& vmId,
                           std::function<void(Result<QJsonObject>)> callback);
    void stopContainer(const QString& node, const QString& vmId,
                       std::function<void(Result<QJsonObject>)> callback);
    void suspendContainer(const QString& node, const QString& vmId,
                          std::function<void(Result<QJsonObject>)> callback);
    void resumeContainer(const QString& node, const QString& vmId,
                         std::function<void(Result<QJsonObject>)> callback);
    void getRrdData(const QString& node, const QString& vmId,
                    std::function<void(Result<QList<RrdData>>)> callback);
    void getVzTemplates(const QString& node, std::function<void(Result<QList<VzTemp>>)> callback);
    void getConfig(const QString& node, const QString& vmId,
                   std::function<void(Result<Config>)> callback);

    // snapshot
    void getSnapshots(const QString& node, const QString& vmId,
                      std::function<void(Result<QList<Snapshot>>)> callback);
    void createSnapshot(const QString& node, const QString& vmId, const QString& snapName,
                        std::function<void(Result<QList<Snapshot>>)> callback);
    void deleteSnapshot(const QString& node, const QString& vmId, const QString& snapName,
                        std::function<void(Result<QList<Snapshot>>)> callback);
    void rollbackSnapshot(const QString& node, const QString& vmId, const QString& snapName,
                          std::function<void(Result<QList<Snapshot>>)> callback);

private:
    QNetworkAccessManager manager;
    QByteArray tokenBytes;
    QByteArray ticket;
    TcpSockify* tcpSockify;
    QNetworkReply* createReply(ContainerDesktop::Method verb, const QNetworkRequest& requestInfo,
                               const QByteArray& body);
};
} // namespace ContainerDesktop
#endif // NETWORKCLIENT_H

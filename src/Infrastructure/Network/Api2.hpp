#include "Infrastructure/Network/NetworkClient.h"
#include "Infrastructure/Utility/CookieUtils.h"
#include "Infrastructure/Utility/Result.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <qjsonobject.h>
#include <qstringliteral.h>
#include <tuple>

constexpr const char USER_AGENT[] = "Client/1";
constexpr const char MIME_TYPE[] = "application/json";

namespace ContainerDesktop {
struct Api2 {
    Api2() = delete;
    ~Api2() = delete;
    static std::tuple<QNetworkRequest, QByteArray> makeRequest(ContainerDesktopDetails::Method verb,
                                                               const QUrl& url,
                                                               QNetworkCookieJar* cookieJar,
                                                               const QJsonDocument& data = {}) {
        QNetworkRequest request;
        request.setHeader(QNetworkRequest::UserAgentHeader, USER_AGENT);
        request.setUrl(url);
        auto cookies = cookieJar->cookiesForUrl(url);
        auto dataObject = data.object();

        auto header =
            QJsonObject{{"PVEAuthCookie", getCookieOrDefault(cookies, "PVEAuthCookie", "")}};
        dataObject.insert("header", header);

        for (auto it = header.begin(); it != header.end(); ++it) {
            if (!containsCookie(cookies, it.key())) {
                QNetworkCookie cookieItem(it.key().toUtf8(), it.value().toString().toUtf8());
                cookieJar->insertCookie(cookieItem);
            }
        }

        if (verb == Method::POST) {
            request.setHeader(QNetworkRequest::ContentTypeHeader, MIME_TYPE);
            auto urlQuery = QJsonDocument{data.object()}.toJson(QJsonDocument::Compact);
            return std::make_tuple(request, urlQuery);
        }

        return std::make_tuple(request, QByteArray());
    }

    static Result<QByteArray> decryptResponse(const QByteArray& contentType, QByteArray&& data) {
        Q_UNUSED(contentType)
        return data;
    }
};
} // namespace ContainerDesktop
#include "Infrastructure/Utility/Result.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkCookieJar>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <qstringliteral.h>
#include <tuple>

constexpr const char USER_AGENT[] = "Client/1";
constexpr const char MIME_TYPE[] = "application/json";

namespace ContainerDesktop {
struct Api2 {
    Api2() = delete;
    ~Api2() = delete;
    static std::tuple<QNetworkRequest, QByteArray>
        makeRequest(const QByteArray& verb, const QUrl& url,
                    QNetworkCookieJar* cookieJar,
                    const QJsonDocument& data = {}) {
        QNetworkRequest request;
        request.setHeader(QNetworkRequest::UserAgentHeader, USER_AGENT);
        request.setUrl(url);

        if (verb.compare("POST", Qt::CaseInsensitive) == 0) {
            request.setHeader(QNetworkRequest::ContentTypeHeader, MIME_TYPE);
            auto urlQuery =
                QJsonDocument{data.object()}.toJson(QJsonDocument::Compact);
            return std::make_tuple(request, urlQuery);
        }

        return std::make_tuple(request, QByteArray());
    }

    static Result<QByteArray> decryptResponse(const QByteArray& contentType,
                                              QByteArray&& data) {
        Q_UNUSED(contentType)
        return data;
    }
};
} // namespace ContainerDesktop
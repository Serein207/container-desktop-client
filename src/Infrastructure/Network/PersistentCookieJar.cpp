#include "PersistentCookieJar.h"
#include "Infrastructure/Cache/CacheManager.h"

PersistentCookieJar::PersistentCookieJar(QObject* parent) : QNetworkCookieJar(parent) {
    load();
}

PersistentCookieJar::~PersistentCookieJar() {}

QList<QNetworkCookie> PersistentCookieJar::cookiesForUrl(const QUrl& url) const {
    QMutexLocker lock(&mutex);
    return allCookies();
}

bool PersistentCookieJar::setCookiesFromUrl(const QList<QNetworkCookie>& cookieList,
                                            const QUrl& url) {
    QMutexLocker lock(&mutex);
    return QNetworkCookieJar::setCookiesFromUrl(cookieList, url);
}

void PersistentCookieJar::save() {
    QMutexLocker lock(&mutex);
    QList<QNetworkCookie> list = allCookies();
    QByteArray data;

    foreach (QNetworkCookie cookie, list) {
        data.append(cookie.toRawForm());
        data.append(";");
    }
    CacheManager::getInstance()->setCookies(data);
}

void PersistentCookieJar::load() {
    QByteArray data = CacheManager::getInstance()->getCookies().value_or("");
    setAllCookies(QNetworkCookie::parseCookies(data));
}

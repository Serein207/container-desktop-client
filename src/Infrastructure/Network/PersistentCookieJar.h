#ifndef PERSISTENT_COOKIE_JAR_H
#define PERSISTENT_COOKIE_JAR_H

#include <QMutex>
#include <QMutexLocker>
#include <QNetworkCookie>
#include <QNetworkCookieJar>

class PersistentCookieJar : public QNetworkCookieJar {
public:
    PersistentCookieJar(QObject* parent = nullptr);
    ~PersistentCookieJar();
    virtual QList<QNetworkCookie> cookiesForUrl(const QUrl& url) const;
    virtual bool setCookiesFromUrl(const QList<QNetworkCookie>& cookieList, const QUrl& url);
    void save();

private:
    void load();
    mutable QMutex mutex;
};

#endif // PERSISTENT_COOKIE_JAR_H
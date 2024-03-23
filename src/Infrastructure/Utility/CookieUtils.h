#ifndef COOKIE_UTILS_H
#define COOKIE_UTILS_H

#include <QNetworkCookieJar>

QString getCookieOrDefault(const QList<QNetworkCookie>& cookies, const QString& name,
                           const QString& defaultValue);
bool containsCookie(const QList<QNetworkCookie>& cookies, const QString& name);

#endif // COOKIE_UTILS_H
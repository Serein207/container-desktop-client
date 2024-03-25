#include "CacheManager.h"
#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <QStandardPaths>
#include <optional>

static auto filepath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) +
                       QStringLiteral("/GaoDiZhengGeGuo/Container Desktop/cache.dat");

CacheManager::CacheManager() {
    qDebug() << filepath;
    if (!loadCacheFromFile(filepath)) {
        cache = QMap<QString, QString>();
    }
}

CacheManager::~CacheManager() {
    if (!saveCacheToFile(filepath))
        qDebug() << "Failed to save cache";
}

CacheManager* CacheManager::getInstance() {
    static CacheManager instance;
    return &instance;
}

void CacheManager::setCookies(const QByteArray& cookie) {
    std::lock_guard<std::shared_mutex> lock(mtx);
    cache["Cookies"] = cookie;
}

std::optional<QByteArray> CacheManager::getCookies() {
    std::shared_lock<std::shared_mutex> lock(mtx);
    if (!cache.contains("Cookies"))
        return std::nullopt;
    auto cookie = cache["Cookies"];
    if (cookie.isEmpty())
        return std::nullopt;
    return cookie.toUtf8();
}

void CacheManager::setNodeForUsername(const QString& username, const QString& node) {
    std::lock_guard<std::shared_mutex> lock(mtx);
    cache[username] = node;
}

std::optional<QString> CacheManager::getNodeFromUsername(const QString& username) {
    std::shared_lock<std::shared_mutex> lock(mtx);
    if (!cache.contains(username)) {
        return std::nullopt;
    }
    return cache[username];
}

void CacheManager::setUsername(const QString& username) {
    std::lock_guard<std::shared_mutex> lock(mtx);
    cache["username"] = username;
}

std::optional<QString> CacheManager::getUsername() {
    std::shared_lock<std::shared_mutex> lock(mtx);
    if (!cache.contains("username"))
        return std::nullopt;
    auto username = cache["username"];
    if (username.isEmpty())
        return std::nullopt;
    return username;
}

bool CacheManager::loadCacheFromFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QDataStream in(&file);
    in >> cache;
    file.close();
    return true;
}

bool CacheManager::saveCacheToFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    QDataStream out(&file);
    out << cache;
    file.close();
    return true;
}
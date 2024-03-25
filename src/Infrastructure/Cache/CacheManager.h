#ifndef CACHE_MANAGER_H
#define CACHE_MANAGER_H

#include <QMap>
#include <QSettings>
#include <shared_mutex>

class CacheManager {
public:
    void setCookies(const QByteArray& cookie);
    std::optional<QByteArray> getCookies();

    void setNodeForUsername(const QString& username, const QString& node);
    std::optional<QString> getNodeFromUsername(const QString& username);

    void setUsername(const QString& username);
    std::optional<QString> getUsername();

    static CacheManager* getInstance();
    ~CacheManager();

private:
    CacheManager();

    bool loadCacheFromFile(const QString& filPath);
    bool saveCacheToFile(const QString& filePath);

    QMap<QString, QString> cache;
    std::shared_mutex mtx;
};

#endif // CACHE_MANAGER_H
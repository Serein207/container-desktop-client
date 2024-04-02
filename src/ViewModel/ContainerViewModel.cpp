#include "ContainerViewModel.h"
#include "Infrastructure/Cache/CacheManager.h"
#include "Infrastructure/Network/NetworkClient.h"

ContainerViewModel::ContainerViewModel(QObject* parent) : QObject(parent) {}

void ContainerViewModel::start(const QString& vmId) {
    auto vmIdInt = vmId.toInt();
    if (vmIdInt > 256 * 255 + 1000) {
        emit loadFailed("vmId is invalid!");
        return;
    }
    if (containerStatus.contains(vmIdInt) && containerStatus[vmIdInt] == Status::Running) {
        emit loadFailed("vm is running!");
        return;
    }
    ContainerDesktop::NetworkClient::getInstance()->startContainer(
        CacheManager::getInstance()->getNodeForCurrentUser().value(), vmId,
        [this, vmIdInt](Result<QJsonObject> result) {
            if (result.isErr()) {
                emit loadFailed(result.unwrapErr().message);
                return;
            }
            emit loadSuccess();
            containerStatus[vmIdInt] = Status::Running;
        });
}

void ContainerViewModel::close(const QString& vmId) {
    auto vmIdInt = vmId.toInt();
    if (!containerStatus.contains(vmIdInt) || containerStatus[vmIdInt] == Status::Stopped) {
        emit loadFailed("vm is closed!");
        return;
    }
    ContainerDesktop::NetworkClient::getInstance()->stopContainer(
        CacheManager::getInstance()->getNodeForCurrentUser().value(), vmId,
        [this, vmIdInt](Result<QJsonObject> result) {
            if (result.isErr()) {
                emit loadFailed(result.unwrapErr().message);
                return;
            }
            emit loadSuccess();
        });
    containerStatus[vmIdInt] = Status::Stopped;
}

void ContainerViewModel::connectVnc(const QString& vmId) {
    auto vmIdInt = vmId.toInt();
    auto ip =
        QStringLiteral("172.18.%1.%2").arg(1 + (vmIdInt - 1000) / 256).arg((vmIdInt - 1000) % 256);
    ContainerDesktop::NetworkClient::getInstance()->connectVnc(ip, 6001);
}

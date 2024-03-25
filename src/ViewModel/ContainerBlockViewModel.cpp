#include "ContainerBlockViewModel.h"
#include "Infrastructure/Cache/CacheManager.h"
#include "Infrastructure/Network/NetworkClient.h"
#include "Infrastructure/Utility/Result.hpp"
#include "Infrastructure/Utility/Tools.h"

ContainerBlockViewModel::ContainerBlockViewModel(QObject* parent) : QAbstractListModel(parent) {}

int ContainerBlockViewModel::rowCount(const QModelIndex& parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return model.count();
}

QVariant ContainerBlockViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();
    auto element = model[index.row()];
    switch (role) {
    case Role::VmId:
        return element.vmId;
    case Role::Name:
        return element.name;
    case Role::Cpu:
        return std::round(element.cpu * 10000) / 100;
    case Role::Mem:
        return Tools::formatBytes(element.mem) + " / " + Tools::formatBytes(element.maxMem);
    case Role::Status:
        return element.status;
    case Role::Remark:
        return QStringLiteral("上课时用的系统");
    }
    return QVariant();
}

QHash<int, QByteArray> ContainerBlockViewModel::roleNames() const {
    static QHash<int, QByteArray> roles;
    if (roles.isEmpty()) {
        roles[Role::VmId] = "vmId";
        roles[Role::Name] = "name";
        roles[Role::Cpu] = "cpu";
        roles[Role::Mem] = "mem";
        roles[Role::Status] = "status";
        roles[Role::Remark] = "remark";
    }
    return roles;
}

void ContainerBlockViewModel::resetModel(QList<ContainerBlock> newModel) {
    beginResetModel();
    model = std::move(newModel);
    endResetModel();
}

void ContainerBlockViewModel::load() {
    auto username = CacheManager::getInstance()->getUsername();
    auto nodeName = CacheManager::getInstance()->getNodeFromUsername(username.value_or("INVALID"));
    if (nodeName.has_value()) {
        ContainerDesktop::NetworkClient::getInstance()->getAllContainerInfo(
            nodeName.value(), [this](Result<QList<ContainerBlock>> result) {
                if (result.isErr()) {
                    emit loadFailed(result.unwrapErr().message);
                    return;
                }
                resetModel(std::move(result.unwrap()));
                emit loadSuccess();
            });
        return;
    }
    ContainerDesktop::NetworkClient::getInstance()->getClusterStatusInfo(
        [this](Result<QJsonArray> result) {
            if (result.isErr()) {
                emit loadFailed(result.unwrapErr().message);
                return;
            }
            auto data = result.unwrap();
            if (data.empty()) {
                emit loadFailed("无法获取节点数据");
                return;
            }
            for (const auto& value : data) {
                auto obj = value.toObject();
                if (parseData(obj))
                    return;
            }
            emit loadFailed("无法获取节点数据");
        });
}

bool ContainerBlockViewModel::parseData(const QJsonObject& obj) {
    QString node = "";
    if (obj.contains("type") && !obj["type"].isNull()) {
        if (obj["type"].toString() == QStringLiteral("node")) {
            if (obj.contains("name") && !obj["name"].isNull()) {
                node = obj["name"].toString();
            }
        }
    }
    if (node == "")
        return false;
    auto username = CacheManager::getInstance()->getUsername();
    CacheManager::getInstance()->setNodeForUsername(username.value(), node);
    ContainerDesktop::NetworkClient::getInstance()->getAllContainerInfo(
        node, [this](Result<QList<ContainerBlock>> result) {
            if (result.isErr()) {
                emit loadFailed(result.unwrapErr().message);
                return;
            }
            resetModel(std::move(result.unwrap()));
            emit loadSuccess();
        });
    return true;
}

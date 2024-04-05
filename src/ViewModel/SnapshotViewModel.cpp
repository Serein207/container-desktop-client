#include "SnapshotViewModel.h"
#include "Infrastructure/Cache/CacheManager.h"
#include "Infrastructure/Network/NetworkClient.h"
#include "Infrastructure/Utility/Result.hpp"
#include "Model/Snapshot.h"

SnapshotViewModel::SnapshotViewModel(QObject* parent) : QAbstractListModel(parent) {}

int SnapshotViewModel::rowCount(const QModelIndex& parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return model.count();
}

QVariant SnapshotViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();
    auto element = model[index.row()];
    switch (role) {
    case Role::Name:
        return element.name;
    case Role::Description:
        return element.description;
    }
    return QVariant();
}

QHash<int, QByteArray> SnapshotViewModel::roleNames() const {
    static QHash<int, QByteArray> roles;
    if (roles.isEmpty()) {
        roles[Role::Name] = "name";
        roles[Role::Description] = "description";
    }
    return roles;
}

void SnapshotViewModel::resetModel(QList<Snapshot> newModel) {
    beginResetModel();
    model = std::move(newModel);
    endResetModel();
}

void SnapshotViewModel::load(const QString& vmId) {
    ContainerDesktop::NetworkClient::getInstance()->getSnapshots(
        CacheManager::getInstance()->getNodeForCurrentUser().value(), vmId,
        [this](Result<QList<Snapshot>> result) {
            if (result.isErr()) {
                emit loadFailed(result.unwrapErr().message);
                return;
            }
            resetModel(std::move(result.unwrap()));
        });
}

void SnapshotViewModel::createSnapshot(const QString& vmId, const QString& snapName) {
    ContainerDesktop::NetworkClient::getInstance()->createSnapshot(
        CacheManager::getInstance()->getNodeForCurrentUser().value(), vmId, snapName,
        [this, vmId](Result<QList<Snapshot>> result) {
            if (result.unwrapErr().kind == ErrorKind::JsonParseError) {
                load(vmId);
                loadSuccess();
                return;
            }
            emit loadFailed(result.unwrapErr().message);
            return;
        });
}

void SnapshotViewModel::rollbackSnapshot(const QString& vmId, const QString& snapName) {
    ContainerDesktop::NetworkClient::getInstance()->rollbackSnapshot(
        CacheManager::getInstance()->getNodeForCurrentUser().value(), vmId, snapName,
        [this, vmId](Result<QList<Snapshot>> result) {
            if (result.unwrapErr().kind == ErrorKind::JsonParseError) {
                load(vmId);
                loadSuccess();
                return;
            }
            emit loadFailed(result.unwrapErr().message);
            return;
        });
}

void SnapshotViewModel::deleteSnapshot(const QString& vmId, const QString& snapName) {
    ContainerDesktop::NetworkClient::getInstance()->deleteSnapshot(
        CacheManager::getInstance()->getNodeForCurrentUser().value(), vmId, snapName,
        [this, vmId, snapName](Result<QList<Snapshot>> result) {
            if (result.unwrapErr().kind == ErrorKind::JsonParseError) {
                beginResetModel();
                model.erase(std::remove_if(
                                model.begin(), model.end(),
                                [snapName](const Snapshot& snap) { return snap.name == snapName; }),
                            model.end());
                endResetModel();
                loadSuccess();
                return;
            }
            emit loadFailed(result.unwrapErr().message);
        });
}

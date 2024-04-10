#include "VzTemplateViewModel.h"
#include "Infrastructure/Cache/CacheManager.h"
#include "Infrastructure/Network/NetworkClient.h"
#include "Infrastructure/Utility/Result.hpp"
#include "Model/VzTemp.h"

VzTemplateViewModel::VzTemplateViewModel(QObject* parent) : QAbstractListModel(parent) {}

int VzTemplateViewModel::rowCount(const QModelIndex& parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return model.count();
}

QVariant VzTemplateViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();
    auto element = model[index.row()];
    switch (role) {
    case Role::Name:
        return element.name;
    case Role::Description:
        return element.description;
    case Role::OsTemplate:
        return element.osTemplate;
    case Role::OsType:
        return element.osType;
    case Role::Url:
        return element.url;
    }
    return QVariant();
}

QHash<int, QByteArray> VzTemplateViewModel::roleNames() const {
    static QHash<int, QByteArray> roles;
    if (roles.isEmpty()) {
        roles[Role::Name] = "name";
        roles[Role::Description] = "description";
        roles[Role::OsTemplate] = "osTemplate";
        roles[Role::OsType] = "osType";
        roles[Role::Url] = "url";
    }
    return roles;
}

void VzTemplateViewModel::resetModel(QList<VzTemp> newModel) {
    beginResetModel();
    model = std::move(newModel);
    endResetModel();
}

void VzTemplateViewModel::load() {
    beginResetModel();
    model = QList<VzTemp>{
        VzTemp{
            "ubuntuKylin",
            "银河麒麟建设自主的开源供应链，发起中国首个开源桌面操作系统根社区openKylin，银河麒麟操作系统以openKylin等自主根社区为依托，发布最新版本",
            "local:vztmpl/ubuntukylin-22.04-mini-20240329.tar.zst", "kylinOS",
            "qrc:/res/img/template-kylin.png"},
        VzTemp{
            "ubuntu",
            "Ubuntu 是一个基于Debian 的Linux 发行版，完全自由开源，以其桌面版而闻名，它直观、用户友好，被认为是学者和初学者的理想选择",
            "", "", "qrc:/res/img/template-ubuntu.png"},
        VzTemp{
            "统信uos",
            "统信uos是一款基于Debian的Linux操作系统。  统信uos借鉴了Debian的优秀设计和技术，结合自身特点进行了精简和优化，使其成为一款健壮、安全、易用的操作系统。",
            "", "", "qrc:/res/img/txos.png"}};
    endResetModel();
    emit loadSuccess();
    // ContainerDesktop::NetworkClient::getInstance()->getVzTemplates(
    //     CacheManager::getInstance()->getNodeForCurrentUser().value(),
    //     [this](Result<QList<VzTemp>> result) {
    //         if (result.isErr()) {
    //             emit loadFailed(result.unwrapErr().message);
    //             return;
    //         }
    //         resetModel(std::move(result.unwrap()));
    //         emit loadSuccess();
    //     });
}

void VzTemplateViewModel::createContainer(const QString& osTemplate, const QString& userPassword,
                                          const QString& vncPassword) {
    ContainerDesktop::NetworkClient::getInstance()->createContainer(
        CacheManager::getInstance()->getNodeForCurrentUser().value(), osTemplate, userPassword,
        vncPassword, [this](Result<QJsonObject> result) {
            if (result.isErr()) {
                emit createFailed(result.unwrapErr().message);
                return;
            }
            emit createSuccess();
        });
}

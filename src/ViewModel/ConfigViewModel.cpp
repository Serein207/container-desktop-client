#include "ConfigViewModel.h"
#include "Infrastructure/Cache/CacheManager.h"
#include "Infrastructure/Network/NetworkClient.h"

ConfigViewModel::ConfigViewModel(QObject* parent) : QObject(parent) {}

void ConfigViewModel::load(const QString& vmId) {
    ContainerDesktop::NetworkClient::getInstance()->getConfig(
        CacheManager::getInstance()->getNodeForCurrentUser().value(), vmId,
        [this](Result<Config> result) {
            if (result.isErr()) {
                emit loadFailed(result.unwrapErr().message);
                return;
            }
            auto config = result.unwrap();
            setHostname(config.hostname);
            setArch(config.arch);
            setOstype(config.ostype);
            setRootfs(config.rootfs);
            setCores(config.cores);
            setMemory(config.memory);
            emit loadSuccess();
        });
}

QString ConfigViewModel::getHostname() const {
    return hostname;
}

void ConfigViewModel::setHostname(const QString& newHostname) {
    if (hostname == newHostname)
        return;
    hostname = newHostname;
    emit hostnameChanged();
}

QString ConfigViewModel::getArch() const {
    return arch;
}

void ConfigViewModel::setArch(const QString& newArch) {
    if (arch == newArch)
        return;
    arch = newArch;
    emit archChanged();
}

QString ConfigViewModel::getOstype() const {
    return ostype;
}

void ConfigViewModel::setOstype(const QString& newOstype) {
    if (ostype == newOstype)
        return;
    ostype = newOstype;
    emit ostypeChanged();
}

QString ConfigViewModel::getRootfs() const {
    return rootfs;
}

void ConfigViewModel::setRootfs(const QString& newRootfs) {
    if (rootfs == newRootfs)
        return;
    rootfs = newRootfs;
    emit rootfsChanged();
}

int ConfigViewModel::getCores() const {
    return cores;
}

void ConfigViewModel::setCores(int newCores) {
    if (cores == newCores)
        return;
    cores = newCores;
    emit coresChanged();
}

int ConfigViewModel::getMemory() const {
    return memory;
}

void ConfigViewModel::setMemory(int newMemory) {
    if (memory == newMemory)
        return;
    memory = newMemory;
    emit memoryChanged();
}

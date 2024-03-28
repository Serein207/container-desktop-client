#include "ContainerViewModel.h"
#include "Infrastructure/Network/NetworkClient.h"

ContainerViewModel::ContainerViewModel(QObject* parent) : QObject(parent) {}

void ContainerViewModel::load(const QString& vmId) {
    connectVnc(QStringLiteral("172.18.1.") + QString::number(vmId.toInt() - 1000), 6001);
    emit loadSuccess();
}

void ContainerViewModel::connectVnc(const QString& ip, unsigned short port) {
    ContainerDesktop::NetworkClient::getInstance()->connectVnc(ip, port);
}

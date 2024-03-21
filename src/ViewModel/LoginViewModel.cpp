#include "LoginViewModel.h"
#include <Infrastructure/Network/NetworkClient.h>

LoginViewModel::LoginViewModel(QObject* parent) {}

void LoginViewModel::login(const QString& username, const QString& password) {
    ContainerDesktop::NetworkClient::getInstance()->login(
        username, password, [this](Result<QJsonObject> result) {
            if (result.isErr()) {
                emit loginFailed(result.unwrapErr().message);
                return;
            }
            emit loginSuccess();
        });
}
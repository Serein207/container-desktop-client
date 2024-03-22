#include "LoginViewModel.h"
#include <Infrastructure/Network/NetworkClient.h>

LoginViewModel::LoginViewModel(QObject* parent) {}

void LoginViewModel::login(QStringView username, QStringView password) {
    ContainerDesktop::NetworkClient::getInstance()->login(
        username, password, [this](Result<User> result) {
            if (result.isErr()) {
                emit loginFailed(result.unwrapErr().message);
                return;
            }
            auto user = result.unwrap();
            emit loginSuccess();
        });
}
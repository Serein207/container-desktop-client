#include "LoginViewModel.h"
#include "Infrastructure/Utility/Result.hpp"
#include <Infrastructure/Network/NetworkClient.h>
#include <qjsonobject.h>

LoginViewModel::LoginViewModel(QObject* parent) : timer(new QTimer(this)) {
    using namespace std::chrono_literals;
    username = settings.value("username", "").toString();
    timer->setInterval(110min);
    connect(timer, &QTimer::timeout, this, &LoginViewModel::updateStatus);
}

LoginViewModel::~LoginViewModel() {
    settings.setValue("username", username);
}

void LoginViewModel::login(const QString& username, const QString& password) {
    ContainerDesktop::NetworkClient::getInstance()->login(
        username, password, [this](Result<User> result) {
            if (result.isErr()) {
                emit loginFailed("账号或密码不正确");
                return;
            }
            auto user = result.unwrap();
            setUsername(user.username.mid(0, user.username.length() - 4));
            emit loginSuccess();
        });
}

void LoginViewModel::registerUser(const QString& username, const QString& password) {
    ContainerDesktop::NetworkClient::getInstance()->registerUser(
        username, password, [this](Result<QJsonObject> result) {
            if (result.isErr()) {
                emit loginFailed(result.unwrapErr().message);
                return;
            }
            emit loginSuccess();
        });
}

QString LoginViewModel::getUsername() const {
    return username;
}

void LoginViewModel::setUsername(const QString& newUsername) {
    if (username == newUsername)
        return;
    username = newUsername;
    emit usernameChanged();
}

void LoginViewModel::updateStatus() {
    ContainerDesktop::NetworkClient::getInstance()->login(
        username,
        QString::fromStdString(
            ContainerDesktop::NetworkClient::getInstance()->getTicket().toStdString()),
        [this](Result<User> result) {
            if (result.isErr()) {
                emit loginFailed("登录已过期，请重新登录");
                return;
            }
            emit loginSuccess();
        });
}

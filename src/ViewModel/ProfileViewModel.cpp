#include "ProfileViewModel.h"
#include "Infrastructure/Cache/CacheManager.h"
#include "Infrastructure/Network/NetworkClient.h"

ProfileViewModel::ProfileViewModel(QObject* parent) : QObject(parent) {}

void ProfileViewModel::load() {
    ContainerDesktop::NetworkClient::getInstance()->getProfile([this](Result<Profile> result) {
        if (result.isErr()) {
            emit loadFailed(result.unwrapErr().message);
            return;
        }
        auto profile = result.unwrap();
        setEmail(profile.email.value_or(""));
        setFirstName(profile.firstName.value_or(""));
        setLastName(profile.lastName.value_or(""));
        emit loadSuccess();
    });
}

void ProfileViewModel::updateProfile(const QString& newEmail, const QString& newFirstName,
                                     const QString& newLastName) {
    ContainerDesktop::NetworkClient::getInstance()->modifyProfile(
        CacheManager::getInstance()->getUsername().value(), newEmail, newFirstName, newLastName,
        [this](Result<QJsonObject> result) {
            if (result.isErr()) {
                emit updateProfileFailed(result.unwrapErr().message);
                return;
            }
            emit updateProfileSuccess();
        });
}

void ProfileViewModel::updatePassword(const QString& newPassword) {
    ContainerDesktop::NetworkClient::getInstance()->modifyPassword(
        CacheManager::getInstance()->getUsername().value(), newPassword,
        [this](Result<QJsonObject> result) {
            if (result.isErr()) {
                emit updatePasswordFailed(result.unwrapErr().message);
                return;
            }
            emit updatePasswordSuccess();
        });
}

QString ProfileViewModel::getEmail() const {
    return email;
}

void ProfileViewModel::setEmail(const QString& newEmail) {
    if (email == newEmail)
        return;
    email = newEmail;
    emit emailChanged();
}

QString ProfileViewModel::getFirstName() const {
    return firstName;
}

void ProfileViewModel::setFirstName(const QString& newFirstName) {
    if (firstName == newFirstName)
        return;
    firstName = newFirstName;
    emit firstNameChanged();
}

QString ProfileViewModel::getLastName() const {
    return lastName;
}

void ProfileViewModel::setLastName(const QString& newLastName) {
    if (lastName == newLastName)
        return;
    lastName = newLastName;
    emit lastNameChanged();
}

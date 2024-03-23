#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include "Infrastructure/Utility/Result.hpp"
#include "Model/User.h"
#include <QJsonObject>
#include <QJsonValue>

template <typename T>
struct Deserializer {
    Deserializer() = delete;
};

template <>
struct Deserializer<User> {
    static Result<User> from(const QJsonValue& value) {
        if (!value.isObject()) {
            return ErrorInfo(ErrorKind::JsonDeserializeError, "Invalid JSON type");
        }
        auto obj = value.toObject();
        if (!obj.contains("username")) {
            return ErrorInfo(ErrorKind::JsonDeserializeError, "Invalid JSON type");
        }
        auto username = obj["username"].toString();
        return User{username};
    }
};

#endif // DESERIALIZER_H
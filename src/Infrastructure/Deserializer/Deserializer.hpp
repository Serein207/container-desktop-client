#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include "Infrastructure/Utility/Result.hpp"
#include "Model/Address.h"
#include "Model/ContainerBlock.h"
#include "Model/User.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <qlist.h>

#define CONTAINS_OR_NOT_NULL(field)                                                                \
    if (!obj.contains(field) || obj[field].isNull())                                               \
    return ErrorInfo(ErrorKind::JsonDeserializeError, "Invalid JSON type")

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
        CONTAINS_OR_NOT_NULL("username");
        auto username = obj["username"].toString();
        return User{username};
    }
};

template <>
struct Deserializer<ContainerBlock> {
    static Result<ContainerBlock> from(const QJsonValue& value) {
        if (!value.isObject()) {
            return ErrorInfo(ErrorKind::JsonDeserializeError, "Invalid JSON type");
        }
        auto obj = value.toObject();
        CONTAINS_OR_NOT_NULL("vmid");
        auto vmId = obj["vmid"].toString();
        CONTAINS_OR_NOT_NULL("name");
        auto name = obj["name"].toString();
        CONTAINS_OR_NOT_NULL("mem");
        auto mem = obj["mem"].toInteger();
        CONTAINS_OR_NOT_NULL("maxmem");
        auto maxMem = obj["maxmem"].toInteger();
        CONTAINS_OR_NOT_NULL("cpu");
        auto cpu = obj["cpu"].toDouble();
        CONTAINS_OR_NOT_NULL("status");
        auto status = obj["status"].toString();
        return ContainerBlock{vmId, name, status, (quint64)mem, (quint64)maxMem, cpu};
    }
};

template <>
struct Deserializer<QList<ContainerBlock>> {
    static Result<QList<ContainerBlock>> from(const QJsonValue& value) {
        if (!value.isArray()) {
            return ErrorInfo(ErrorKind::JsonDeserializeError, "Invalid JSON type");
        }
        auto array = value.toArray();
        QList<ContainerBlock> ret;
        for (const auto& obj : array) {
            auto result = Deserializer<ContainerBlock>::from(obj.toObject());
            if (result.isErr())
                return Result<QList<ContainerBlock>>(
                    ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"});
            ret.append(result.unwrap());
        }
        return ret;
    }
};

template <>
struct Deserializer<Address> {
    static Result<Address> from(const QJsonValue& value) {
        if (!value.isObject()) {
            return ErrorInfo(ErrorKind::JsonDeserializeError, "Invalid JSON type");
        }
        auto obj = value.toObject();
        CONTAINS_OR_NOT_NULL("name");
        auto name = obj["name"].toString();
        CONTAINS_OR_NOT_NULL("inet");
        auto ipv4 = obj["inet"].toString();
        CONTAINS_OR_NOT_NULL("inet6");
        auto ipv6 = obj["inet6"].toString();
        CONTAINS_OR_NOT_NULL("hwaddr");
        auto mac = obj["hwaddr"].toString();
        return Address{name, ipv4, ipv6, mac};
    }
};

template <>
struct Deserializer<QList<Address>> {
    static Result<QList<Address>> from(const QJsonValue& value) {
        if (!value.isArray()) {
            return ErrorInfo(ErrorKind::JsonDeserializeError, "Invalid JSON type");
        }
        auto array = value.toArray();
        QList<Address> ret;
        for (const auto& obj : array) {
            auto result = Deserializer<Address>::from(obj.toObject());
            if (result.isErr())
                return Result<QList<Address>>(
                    ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"});
            ret.append(result.unwrap());
        }
        return ret;
    }
};

#endif // DESERIALIZER_H
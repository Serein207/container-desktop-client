#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include "Infrastructure/Utility/Result.hpp"
#include "Model/Address.h"
#include "Model/Config.h"
#include "Model/ContainerBlock.h"
#include "Model/Profile.h"
#include "Model/RrdData.h"
#include "Model/Snapshot.h"
#include "Model/Status.h"
#include "Model/User.h"
#include "Model/VzTemp.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <optional>

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

template <>
struct Deserializer<RrdData> {
    static Result<RrdData> from(const QJsonValue& value) {
        if (!value.isObject()) {
            return ErrorInfo(ErrorKind::JsonDeserializeError, "Invalid JSON type");
        }
        auto obj = value.toObject();
        CONTAINS_OR_NOT_NULL("time");
        auto time = QDateTime::fromSecsSinceEpoch(obj["time"].toInteger());
        CONTAINS_OR_NOT_NULL("netin");
        auto netIn = obj["netin"].toDouble();
        CONTAINS_OR_NOT_NULL("netout");
        auto netOut = obj["netout"].toDouble();
        CONTAINS_OR_NOT_NULL("cpu");
        auto cpu = obj["cpu"].toDouble();
        CONTAINS_OR_NOT_NULL("mem");
        auto memUsed = obj["mem"].toDouble();
        CONTAINS_OR_NOT_NULL("maxmem");
        auto memMax = obj["maxmem"].toInteger();
        return RrdData{time, cpu, netIn, netOut, memUsed, memMax};
    }
};

template <>
struct Deserializer<QList<RrdData>> {
    static Result<QList<RrdData>> from(const QJsonValue& value) {
        if (!value.isArray()) {
            return ErrorInfo(ErrorKind::JsonDeserializeError, "Invalid JSON type");
        }
        auto array = value.toArray();
        QList<RrdData> ret;
        for (const auto& obj : array) {
            auto result = Deserializer<RrdData>::from(obj.toObject());
            if (result.isErr())
                return Result<QList<RrdData>>(
                    ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"});
            ret.append(result.unwrap());
        }
        return ret;
    }
};

template <>
struct Deserializer<Snapshot> {
    static Result<Snapshot> from(const QJsonValue& value) {
        if (!value.isObject()) {
            return ErrorInfo(ErrorKind::JsonDeserializeError, "Invalid JSON type");
        }
        auto obj = value.toObject();
        CONTAINS_OR_NOT_NULL("name");
        auto name = obj["name"].toString();
        CONTAINS_OR_NOT_NULL("description");
        auto description = obj["description"].toString();
        return Snapshot{name, description};
    }
};

template <>
struct Deserializer<QList<Snapshot>> {
    static Result<QList<Snapshot>> from(const QJsonValue& value) {
        if (!value.isArray()) {
            return ErrorInfo(ErrorKind::JsonDeserializeError, "Invalid JSON type");
        }
        auto array = value.toArray();
        QList<Snapshot> ret;
        for (const auto& obj : array) {
            auto result = Deserializer<Snapshot>::from(obj.toObject());
            if (result.isErr())
                return Result<QList<Snapshot>>(
                    ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"});
            ret.append(result.unwrap());
        }
        return ret;
    }
};

template <>
struct Deserializer<VzTemp> {
    static Result<VzTemp> from(const QJsonValue& value) {
        if (!value.isObject()) {
            return ErrorInfo(ErrorKind::JsonDeserializeError, "Invalid JSON type");
        }
        auto obj = value.toObject();
        CONTAINS_OR_NOT_NULL("name");
        auto name = obj["name"].toString();
        CONTAINS_OR_NOT_NULL("description");
        auto description = obj["description"].toString();
        CONTAINS_OR_NOT_NULL("ostemplate");
        auto osTemplate = obj["ostemplate"].toString();
        CONTAINS_OR_NOT_NULL("ostype");
        auto osType = obj["ostype"].toString();
        return VzTemp{name, description, osTemplate, osType};
    }
};

template <>
struct Deserializer<QList<VzTemp>> {
    static Result<QList<VzTemp>> from(const QJsonValue& value) {
        if (!value.isArray()) {
            return ErrorInfo(ErrorKind::JsonDeserializeError, "Invalid JSON type");
        }
        auto array = value.toArray();
        QList<VzTemp> ret;
        for (const auto& obj : array) {
            auto result = Deserializer<VzTemp>::from(obj.toObject());
            if (result.isErr())
                return Result<QList<VzTemp>>(
                    ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"});
            ret.append(result.unwrap());
        }
        return ret;
    }
};

template <>
struct Deserializer<Config> {
    static Result<Config> from(const QJsonValue& value) {
        if (!value.isObject()) {
            return ErrorInfo(ErrorKind::JsonDeserializeError, "Invalid JSON type");
        }
        auto obj = value.toObject();
        CONTAINS_OR_NOT_NULL("hostname");
        auto hostname = obj["hostname"].toString();
        CONTAINS_OR_NOT_NULL("arch");
        auto arch = obj["arch"].toString();
        CONTAINS_OR_NOT_NULL("ostype");
        auto ostype = obj["ostype"].toString();
        CONTAINS_OR_NOT_NULL("rootfs");
        auto rootfs = obj["rootfs"].toString();
        CONTAINS_OR_NOT_NULL("cores");
        auto cores = obj["cores"].toInt();
        CONTAINS_OR_NOT_NULL("memory");
        auto memory = obj["memory"].toInt();
        return Config{hostname, arch, ostype, rootfs, cores, memory};
    }
};

template <>
struct Deserializer<Profile> {
    static Result<Profile> from(const QJsonValue& value) {
        if (!value.isObject()) {
            return ErrorInfo(ErrorKind::JsonDeserializeError, "Invalid JSON type");
        }
        auto obj = value.toObject();
        std::optional<QString> email = std::nullopt;
        if (obj.contains("email") && !obj["email"].isNull())
            email = obj["email"].toString();
        std::optional<QString> firstName = std::nullopt;
        if (obj.contains("firstName") && !obj["firstName"].isNull())
            email = obj["firstName"].toString();
        std::optional<QString> lastName = std::nullopt;
        if (obj.contains("lastName") && !obj["lastName"].isNull())
            email = obj["lastName"].toString();
        return Profile{email, firstName, lastName};
    }
};

template <>
struct Deserializer<Status> {
    static Result<Status> from(const QJsonValue& value) {
        if (!value.isObject()) {
            return ErrorInfo(ErrorKind::JsonDeserializeError, "Invalid JSON type");
        }
        auto obj = value.toObject();
        CONTAINS_OR_NOT_NULL("cpu");
        auto cpu = obj["cpu"].toDouble();
        CONTAINS_OR_NOT_NULL("memory");
        auto memUsed = obj["memory"].toObject()["used"].toInteger();
        auto memMax = obj["memory"].toObject()["max"].toInteger();
        return Status{cpu, memUsed, memMax};
    }
};

#endif // DESERIALIZER_H
#ifndef PROFILE_H
#define PROFILE_H

#include <QString>
#include <optional>

struct Profile {
    std::optional<QString> email;
    std::optional<QString> firstName;
    std::optional<QString> lastName;
};

#endif // PROFILE_Hs
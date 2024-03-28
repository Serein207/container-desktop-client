#ifndef ADDRESS_H
#define ADDRESS_H

#include <QString>

struct Address {
    QString name;
    QString ipv4;
    QString ipv6;
    QString mac;
};

#endif // ADDRESS_H
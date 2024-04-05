#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

struct Config {
    QString hostname;
    QString arch;
    QString ostype;
    QString rootfs;
    int cores;
    int memory;
};

#endif // CONFIG_H
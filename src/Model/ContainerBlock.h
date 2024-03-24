#ifndef CONTAINER_BLOCK_H
#define CONTAINER_BLOCK_H

#include <QString>

struct ContainerBlock {
    QString vmId;
    QString name;
    QString status;
    quint64 mem;
    quint64 maxMem;
    double cpu;
};

#endif // CONTAINER_BLOCK_H
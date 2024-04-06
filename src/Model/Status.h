#ifndef STATUS_H
#define STATUS_H

#include <QString>
struct Status {
    double cpu;
    qint64 memUsed;
    qint64 memMax;
};

#endif // STATUS_H
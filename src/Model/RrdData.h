#ifndef RRD_DATA_H
#define RRD_DATA_H

#include <QDateTime>
struct RrdData {
    QDateTime time;
    double cpu;
    double netIn;
    double netOut;
    double memUsed;
    qint64 memMax;
};

#endif // RRD_DATA_H

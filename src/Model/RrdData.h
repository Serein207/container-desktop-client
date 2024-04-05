#ifndef RRD_DATA_H
#define RRD_DATA_H

#include <QDateTime>
struct RrdData {
    QDateTime time;
    double cpu;
    double ioWait;
    double netIn;
    double netOut;
    double memUsed;
};

#endif // RRD_DATA_H

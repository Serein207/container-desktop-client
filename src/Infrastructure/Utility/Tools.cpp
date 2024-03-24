#include "Tools.h"

QString Tools::formatBytes(quint64 bytes) {
    QStringList suffixes = {"B", "KB", "MB", "GB", "TB"};
    qint64 suffixIndex = 0;

    double size = bytes;
    while (size >= 1024 && suffixIndex < suffixes.size() - 1) {
        size /= 1024;
        ++suffixIndex;
    }

    QString result = QStringLiteral("%1 %2").arg(size, 0, 'f', 2).arg(suffixes[suffixIndex]);
    return result;
}
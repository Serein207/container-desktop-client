#ifndef CONTAINER_VIEW_MODEL_H
#define CONTAINER_VIEW_MODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QtQml/QQmlEngine>
#include <qtmetamacros.h>

class ContainerViewModel : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(ContainerViewModel)
    QML_SINGLETON

public:
    explicit ContainerViewModel(QObject* parent = nullptr);

    Q_INVOKABLE void start(const QString& vmId);
    Q_INVOKABLE void close(const QString& vmId);
    Q_INVOKABLE void connectVnc(const QString& vmId);

signals:
    void loadSuccess();
    void loadFailed(QString message);

private:
    enum class Status { Stopped, Running };

    QHash<int, Status> containerStatus;
};

#endif // CONTAINER_VIEW_MODEL_H

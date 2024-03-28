#ifndef CONTAINER_VIEW_MODEL_H
#define CONTAINER_VIEW_MODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QtQml/QQmlEngine>

class ContainerViewModel : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(ContainerViewModel)
    QML_SINGLETON

public:
    explicit ContainerViewModel(QObject* parent = nullptr);

    Q_INVOKABLE void load(const QString& vmId);

signals:
    void loadSuccess();
    void loadFailed(QString message);

private:
    void connectVnc(const QString& ip, unsigned short port);
};

#endif // CONTAINER_VIEW_MODEL_H

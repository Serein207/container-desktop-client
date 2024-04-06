#ifndef CONTAINERBLOCKVIEWMODEL_H
#define CONTAINERBLOCKVIEWMODEL_H

#include "Model/ContainerBlock.h"
#include <QAbstractListModel>
#include <QList>
#include <QtQml/QQmlEngine>

class ContainerBlockViewModel : public QAbstractListModel {
    Q_OBJECT
    QML_NAMED_ELEMENT(ContainerBlockViewModel)
    QML_SINGLETON

public:
    explicit ContainerBlockViewModel(QObject* parent = nullptr);

    enum Role {
        VmId = Qt::UserRole,
        Name,
        Cpu,
        Mem,
        Remark,
        Status,
    };

    // Basic functionality:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void resetModel(QList<ContainerBlock> newModel);

    Q_INVOKABLE void load();
    Q_INVOKABLE QString getFirstVmId();
    Q_INVOKABLE int getRunningVmNum();

private:
    bool parseData(const QJsonObject& obj);

    QList<ContainerBlock> model;

signals:
    void loadSuccess();
    void loadFailed(QString message);
};

#endif // CONTAINERBLOCKVIEWMODEL_H

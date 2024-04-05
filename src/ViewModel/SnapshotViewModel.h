#ifndef SNAPSHOT_VIEW_MODEL_H
#define SNAPSHOT_VIEW_MODEL_H

#include "Model/Snapshot.h"
#include <QAbstractListModel>
#include <QList>
#include <QtQml/QQmlEngine>

class SnapshotViewModel : public QAbstractListModel {
    Q_OBJECT
    QML_NAMED_ELEMENT(SnapshotViewModel)
    QML_SINGLETON

public:
    explicit SnapshotViewModel(QObject* parent = nullptr);

    enum Role {
        Name = Qt::UserRole,
        Description,
    };

    // Basic functionality:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void resetModel(QList<Snapshot> newModel);

    Q_INVOKABLE void load(const QString& vmId);
    Q_INVOKABLE void createSnapshot(const QString& vmId, const QString& snapName);
    Q_INVOKABLE void rollbackSnapshot(const QString& vmId, const QString& snapName);
    Q_INVOKABLE void deleteSnapshot(const QString& vmId, const QString& snapName);

private:
    QList<Snapshot> model;

signals:
    void loadSuccess();
    void loadFailed(QString message);
};

#endif // SNAPSHOT_VIEW_MODEL_H

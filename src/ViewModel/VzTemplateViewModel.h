#ifndef VZ_TEMPLATE_VIEW_MODEL_H
#define VZ_TEMPLATE_VIEW_MODEL_H

#include "Model/VzTemp.h"
#include <QAbstractListModel>
#include <QList>
#include <QtQml/QQmlEngine>
#include <qtmetamacros.h>

class VzTemplateViewModel : public QAbstractListModel {
    Q_OBJECT
    QML_NAMED_ELEMENT(VzTemplateViewModel)
    QML_SINGLETON

public:
    explicit VzTemplateViewModel(QObject* parent = nullptr);

    enum Role {
        Name = Qt::UserRole,
        Description,
        OsTemplate,
        OsType,
        Url,
    };

    // Basic functionality:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void resetModel(QList<VzTemp> newModel);

    Q_INVOKABLE void load();
    Q_INVOKABLE void createContainer(const QString& osTemplate, const QString& userPassword,
                                     const QString& vncPassword);

private:
    QList<VzTemp> model;

signals:
    void loadSuccess();
    void loadFailed(QString message);
    void createSuccess();
    void createFailed(QString message);
};

#endif // VZ_TEMPLATE_VIEW_MODEL_H

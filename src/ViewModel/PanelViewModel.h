#ifndef PANEL_VIEW_MODEL_H
#define PANEL_VIEW_MODEL_H

#include <QtQml/QQmlEngine>

class PanelViewModel : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(PanelViewModel)
    QML_SINGLETON

public:
    explicit PanelViewModel(QObject* parent = nullptr);

    Q_INVOKABLE void load(const QString& vmId, const QString& timeFrame);

    QString getCurCpu() const;
    void setCurCpu(const QString& newCurCpu);

    QString getCurMemory() const;
    void setCurMemory(const QString& newCurMemory);

    QString getCurNetIn() const;
    void setCurNetIn(const QString& newCurNetIn);

    QString getCurNetOut() const;
    void setCurNetOut(const QString& newCurNetOut);

    QString getArrayTime() const;
    void setArrayTime(const QString& newArrayTime);

    QString getArrayCpu() const;
    void setArrayCpu(const QString& newArrayCpu);

    QString getArrayMemory() const;
    void setArrayMemory(const QString& newArrayMemory);

    QString getArrayNetIn() const;
    void setArrayNetIn(const QString& newArrayNetIn);

    QString getArrayNetOut() const;
    void setArrayNetOut(const QString& newArrayNetOut);

signals:
    void loadSuccess();
    void loadFailed(QString message);

    void loadStatusSuccess();
    void loadStatusFailed(QString message);

    void curCpuChanged();
    void curMemoryChanged();
    void curNetInChanged();
    void curNetOutChanged();
    void arrayTimeChanged();
    void arrayCpuChanged();
    void arrayMemoryChanged();
    void arrayNetInChanged();
    void arrayNetOutChanged();

private:
    QString curCpu;
    QString curMemory;
    QString curNetIn;
    QString curNetOut;
    QString arrayTime;
    QString arrayCpu;
    QString arrayMemory;
    QString arrayNetIn;
    QString arrayNetOut;

    Q_PROPERTY(QString curCpu READ getCurCpu WRITE setCurCpu NOTIFY curCpuChanged FINAL)
    Q_PROPERTY(QString curMemory READ getCurMemory WRITE setCurMemory NOTIFY curMemoryChanged FINAL)
    Q_PROPERTY(QString curNetIn READ getCurNetIn WRITE setCurNetIn NOTIFY curNetInChanged FINAL)
    Q_PROPERTY(QString curNetOut READ getCurNetOut WRITE setCurNetOut NOTIFY curNetOutChanged FINAL)
    Q_PROPERTY(QString arrayTime READ getArrayTime WRITE setArrayTime NOTIFY arrayTimeChanged FINAL)
    Q_PROPERTY(QString arrayCpu READ getArrayCpu WRITE setArrayCpu NOTIFY arrayCpuChanged FINAL)
    Q_PROPERTY(QString arrayMemory READ getArrayMemory WRITE setArrayMemory NOTIFY
                   arrayMemoryChanged FINAL)
    Q_PROPERTY(
        QString arrayNetIn READ getArrayNetIn WRITE setArrayNetIn NOTIFY arrayNetInChanged FINAL)
    Q_PROPERTY(QString arrayNetOut READ getArrayNetOut WRITE setArrayNetOut NOTIFY
                   arrayNetOutChanged FINAL)
};

#endif // PANEL_VIEW_MODEL_H

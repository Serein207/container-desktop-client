#ifndef CONFIG_VIEW_MODEL_H
#define CONFIG_VIEW_MODEL_H

#include <QtQml/QQmlEngine>

class ConfigViewModel : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(ConfigViewModel)
    QML_SINGLETON

public:
    explicit ConfigViewModel(QObject* parent = nullptr);

    Q_INVOKABLE void load(const QString& vmId);

    QString getHostname() const;
    void setHostname(const QString& newHostname);

    QString getArch() const;
    void setArch(const QString& newArch);

    QString getOstype() const;
    void setOstype(const QString& newOstype);

    QString getRootfs() const;
    void setRootfs(const QString& newRootfs);

    int getCores() const;
    void setCores(int newCores);

    int getMemory() const;
    void setMemory(int newMemory);

signals:
    void loadSuccess();
    void loadFailed(QString message);

    void hostnameChanged();
    void archChanged();
    void ostypeChanged();
    void rootfsChanged();
    void coresChanged();
    void memoryChanged();

private:
    QString hostname;
    QString arch;
    QString ostype;
    QString rootfs;
    int cores = 0;
    int memory = 0;
    Q_PROPERTY(QString hostname READ getHostname WRITE setHostname NOTIFY hostnameChanged FINAL)
    Q_PROPERTY(QString arch READ getArch WRITE setArch NOTIFY archChanged FINAL)
    Q_PROPERTY(QString ostype READ getOstype WRITE setOstype NOTIFY ostypeChanged FINAL)
    Q_PROPERTY(QString rootfs READ getRootfs WRITE setRootfs NOTIFY rootfsChanged FINAL)
    Q_PROPERTY(int cores READ getCores WRITE setCores NOTIFY coresChanged FINAL)
    Q_PROPERTY(int memory READ getMemory WRITE setMemory NOTIFY memoryChanged FINAL)
};

#endif // CONFIG_VIEW_MODEL_H

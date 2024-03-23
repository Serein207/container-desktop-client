#ifndef LOGIN_VIEW_MODEL_H
#define LOGIN_VIEW_MODEL_H

#include <QObject>
#include <QSettings>
#include <QTimer>
#include <QtQml/QQmlEngine>
#include <qtmetamacros.h>

class LoginViewModel : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(LoginViewModel)
    QML_SINGLETON

public:
    explicit LoginViewModel(QObject* parent = nullptr);
    ~LoginViewModel();

    Q_INVOKABLE void login(const QString& username, const QString& password);
    Q_INVOKABLE void registerUser(const QString& username, const QString& password);

    QString getUsername() const;
    void setUsername(const QString& newUsername);

signals:
    void loginSuccess();
    void loginFailed(QString message);
    void registerSuccess();
    void registerFailed(QString message);
    void logoutSuccess();
    void logoutFailed(QString message);

    void usernameChanged();

public slots:
    Q_INVOKABLE void updateStatus();

private:
    QTimer* timer;
    QSettings settings;

    QString username;
    Q_PROPERTY(QString username READ getUsername WRITE setUsername NOTIFY usernameChanged FINAL)
};

#endif // LOGIN_VIEW_MODEL_H

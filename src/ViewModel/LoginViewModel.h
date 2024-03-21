#pragma once
#include <QObject>
#include <QtQml/QQmlEngine>

class LoginViewModel : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(LoginViewModel)
    QML_SINGLETON

public:
    explicit LoginViewModel(QObject* parent = nullptr);
    Q_INVOKABLE void login(const QString& username, const QString& password);

signals:
    void loginSuccess();
    void loginFailed(QString message);
    void logoutSuccess();
    void logoutFailed(QString message);
};

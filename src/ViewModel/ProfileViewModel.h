#ifndef PROFILE_VIEW_MODEL_H
#define PROFILE_VIEW_MODEL_H

#include <QtQml/QQmlEngine>
#include <qtmetamacros.h>

class ProfileViewModel : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(ProfileViewModel)
    QML_SINGLETON

public:
    explicit ProfileViewModel(QObject* parent = nullptr);

    Q_INVOKABLE void load();
    Q_INVOKABLE void updateProfile(const QString& newEmail, const QString& newFirstName,
                                   const QString& newLastName);
    Q_INVOKABLE void updatePassword(const QString& newPassword);

    QString getEmail() const;
    void setEmail(const QString& newEmail);

    QString getFirstName() const;
    void setFirstName(const QString& newFirstName);

    QString getLastName() const;
    void setLastName(const QString& newLastName);

signals:
    void loadSuccess();
    void loadFailed(QString message);
    void updateProfileSuccess();
    void updateProfileFailed(QString message);
    void updatePasswordSuccess();
    void updatePasswordFailed(QString message);

    void emailChanged();
    void firstNameChanged();
    void lastNameChanged();

private:
    QString email;
    QString firstName;
    QString lastName;
    Q_PROPERTY(QString email READ getEmail WRITE setEmail NOTIFY emailChanged FINAL)
    Q_PROPERTY(QString firstName READ getFirstName WRITE setFirstName NOTIFY firstNameChanged FINAL)
    Q_PROPERTY(QString lastName READ getLastName WRITE setLastName NOTIFY lastNameChanged FINAL)
};

#endif // PROFILE_VIEW_MODEL_H

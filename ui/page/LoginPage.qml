import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtCore
import container_desktop
import "../component"
import "../window"

Item {
    id: control
    StackView {
        id: stackView
        initialItem: login
        anchors.fill: parent
        anchors.leftMargin: control.width / 2
        Component.onCompleted: {
            if (!LoginViewModel.isCookieExpired)
                push(auto_login)
        }
    }
    Rectangle {
        id: rect
        anchors.fill: parent
        anchors.rightMargin: control.width / 2
        color: "#3F8CFF"
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: parent.height / 4
            text: "Hi! 你好！\n欢迎使用国产桌面管理系统"
            color: "#fff"
            font.bold: true
            font.pointSize: 28
            wrapMode: Text.WordWrap
        }
        Image {
            width: parent.width / 2
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: parent.height / 2
            source: "qrc:/res/img/cloud.png"
            fillMode: Image.PreserveAspectFit
        }
        Shadow {}
    }

    Connections {
        target: LoginViewModel
        function onLoginFailed(message) {
            showError("Error: " + message, 4000)
            if (stackView.currentItem.objectName == auto_login.objectName)
                stackView.pop()
        }
    }

    Connections {
        target: LoginViewModel
        function onLoginSuccess() {
            enterMainPage()
        }
    }

    Connections {
        target: LoginViewModel
        function onRegisterSuccess() {
            showSuccess("注册成功", 4000)
            stackView.pop()
        }
    }

    Connections {
        target: LoginViewModel
        function onRegisterFailed(message) {
            showError("Error: " + message, 4000)
        }
    }

    Component {
        id: auto_login
        Item {
            width: control.width / 2
            height: control.height
            Text {
                id: text_welcome
                text: "欢迎回来！"
                font.pointSize: 24
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: control.height / 5
            }
            Text {
                id: textBox_username
                width: 300
                text: LoginViewModel.username
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: text_welcome.bottom
                anchors.topMargin: 100
            }
            FlatButton {
                text: "更换账号"
                textColor: "#8a999999"
                fontSize: 8
                height: 15
                anchors.top: textBox_username.bottom
                anchors.topMargin: 5
                anchors.right: textBox_username.right
                onClicked: {
                    stackView.push(login)
                }
            }
            RaisedButton {
                id: btn_login
                width: 300
                text: "登录"
                color: "#2196f3"
                textColor: "#fff"
                rippleColor: "#fff"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: textBox_username.bottom
                anchors.topMargin: 50
                onClicked: {
                    loading = true
                    LoginViewModel.updateStatus()
                }
            }
        }
    }

    Component {
        id: login
        Item {
            width: control.width / 2
            height: control.height
            Text {
                id: text_welcome
                text: "欢迎回来！"
                font.pointSize: 24
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: control.height / 5
            }
            TextBox {
                id: textBox_username
                width: 300
                hint: "用户名"
                text: LoginViewModel.username
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: text_welcome.bottom
                anchors.topMargin: 100
                KeyNavigation.tab: textBox_passwd
            }
            TextBox {
                id: textBox_passwd
                width: 300
                hint: "密码"
                echoMode: TextField.Password
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: textBox_username.bottom
                anchors.topMargin: 20
                Keys.onEnterPressed: btn_login.clicked()
                Keys.onReturnPressed: btn_login.clicked()
            }
            FlatButton {
                text: "注册"
                textColor: "#8a999999"
                fontSize: 8
                height: 15
                anchors.top: textBox_passwd.bottom
                anchors.topMargin: 5
                anchors.right: textBox_passwd.right
                onClicked: {
                    stackView.push(register)
                }
            }
            RaisedButton {
                id: btn_login
                width: 300
                text: "登录"
                color: "#2196f3"
                textColor: "#fff"
                rippleColor: "#fff"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: textBox_passwd.bottom
                anchors.topMargin: 50
                enabled: textBox_username.text !== ""
                         && textBox_passwd.text !== ""
                onClicked: {
                    loading = true
                    LoginViewModel.login(textBox_username.text,
                                         textBox_passwd.text)
                }
            }
            Connections {
                target: LoginViewModel
                function onLoginFailed(message) {
                    btn_login.loading = false
                }
            }
        }
    }

    Component {
        id: register
        Item {
            width: control.width / 2
            height: control.height
            TextBox {
                id: textBox_username
                width: 300
                hint: "用户名"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: control.height / 5
            }
            TextBox {
                id: textBox_passwd
                width: 300
                hint: "密码"
                echoMode: TextField.Password
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: textBox_username.bottom
                anchors.topMargin: 20
            }
            TextBox {
                id: textBox_retype
                width: 300
                hint: "确认密码"
                echoMode: TextField.Password
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: textBox_passwd.bottom
                anchors.topMargin: 20
            }
            TextBox {
                id: textBox_email
                width: 300
                hint: "电子邮箱（选填）"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: textBox_retype.bottom
                anchors.topMargin: 20
            }
            FlatButton {
                text: "返回"
                textColor: "#8a999999"
                fontSize: 8
                height: 15
                anchors.top: textBox_email.bottom
                anchors.topMargin: 5
                anchors.right: textBox_email.right
                onClicked: {
                    stackView.pop()
                }
            }
            RaisedButton {
                width: 300
                text: "注册"
                color: "#2196f3"
                textColor: "#fff"
                rippleColor: "#fff"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: textBox_email.bottom
                anchors.topMargin: 50
                enabled: textBox_username.text !== ""
                         && textBox_passwd.text !== ""
                         && textBox_retype.text !== ""
                onClicked: {
                    if (textBox_passwd.text !== textBox_retype.text) {
                        showError("两次密码不一致", 4000)
                    } else {
                        loading = true
                        LoginViewModel.registerUser(textBox_username.text,
                                                    textBox_passwd.text,
                                                    textBox_email.text)
                    }
                }
            }
        }
    }
}

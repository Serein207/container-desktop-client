import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
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
            text: "Text Text Text Text"
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

    Component {
        id: login
        Item {
            width: control.width / 2
            height: control.height
            Text {
                id: text_welcome
                text: "Welcome back!"
                font.pointSize: 24
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: control.height / 5
            }
            TextBox {
                id: textBox_username
                width: 300
                hint: "username"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: text_welcome.bottom
                anchors.topMargin: 100
            }
            TextBox {
                id: textBox_passwd
                width: 300
                hint: "password"
                echoMode: TextField.Password
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: textBox_username.bottom
                anchors.topMargin: 20
            }
            FlatButton {
                text: "register"
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
                text: "login"
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
                    showError("Error: " + message, 4000)
                }
            }
        }
    }

    Connections {
        target: LoginViewModel
        function onLoginSuccess() {
            enterMainPage()
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
                hint: "username"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: control.height / 5
            }
            TextBox {
                id: textBox_passwd
                width: 300
                hint: "password"
                echoMode: TextField.Password
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: textBox_username.bottom
                anchors.topMargin: 20
            }
            TextBox {
                id: textBox_retype
                width: 300
                hint: "retype password"
                echoMode: TextField.Password
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: textBox_passwd.bottom
                anchors.topMargin: 20
            }
            TextBox {
                id: textBox_email
                width: 300
                hint: "email"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: textBox_retype.bottom
                anchors.topMargin: 20
            }
            FlatButton {
                text: "return"
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
                text: "register"
                color: "#2196f3"
                textColor: "#fff"
                rippleColor: "#fff"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: textBox_email.bottom
                anchors.topMargin: 50
            }
        }
    }
}

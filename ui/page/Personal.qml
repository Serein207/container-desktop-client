import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls
import container_desktop
import "../component"

ScrollablePage {
    id: page
    Row {
        spacing: 40
        Area {
            width: (page.width - parent.spacing) / 2 - page.leftRightMargin
            height: 500
            paddings: 30
            Text {
                text: "更改信息"
                font.pointSize: 18
            }
            Column {
                anchors.centerIn: parent
                spacing: 20
                TextBox {
                    id: textBox_firstName
                    width: 300
                    hint: "姓"
                    text: ProfileViewModel.firstName
                }
                TextBox {
                    id: textBox_lastName
                    width: 300
                    hint: "名"
                    text: ProfileViewModel.lastName
                }
                TextBox {
                    id: textBox_email
                    width: 300
                    hint: "邮箱"
                    text: ProfileViewModel.email
                }
            }
            RaisedButton {
                id: btn_profile
                anchors {
                    right: parent.right
                    rightMargin: 30
                    bottom: parent.bottom
                    bottomMargin: 20
                }
                text: "确定"
                color: "#2196f3"
                textColor: "#fff"
                rippleColor: "#fff"
                enabled: textBox_firstName.text !== ProfileViewModel.firstName
                         && textBox_lastName.text !== ProfileViewModel.lastName
                         && textBox_email.text !== ProfileViewModel.email
                onClicked: {
                    loading = true
                    ProfileViewModel.updateProfile(textBox_firstName.text,
                                                   textBox_lastName.text,
                                                   textBox_email.text)
                }
            }
        }
        Area {
            width: (page.width - parent.spacing) / 2 - page.leftRightMargin
            height: 500
            paddings: 30
            Text {
                text: "更改密码"
                font.pointSize: 18
            }
            Column {
                anchors.centerIn: parent
                spacing: 20
                TextBox {
                    width: 300
                    hint: "旧密码"
                    echoMode: TextField.Password
                }
                TextBox {
                    id: textBox_passwd
                    width: 300
                    hint: "新密码"
                    echoMode: TextField.Password
                }
                TextBox {
                    id: textBox_passwdRetype
                    width: 300
                    hint: "确认新密码"
                    echoMode: TextField.Password
                }
            }
            RaisedButton {
                id: btn_passwd
                anchors {
                    right: parent.right
                    rightMargin: 30
                    bottom: parent.bottom
                    bottomMargin: 20
                }
                text: "确定"
                color: "#2196f3"
                textColor: "#fff"
                rippleColor: "#fff"
                enabled: textBox_passwd.text !== ""
                         && textBox_passwd.text === textBox_passwdRetype.text
                onClicked: {
                    loading = true
                    ProfileViewModel.updatePassword(textBox_passwd)
                }
            }
        }
    }

    Connections {
        target: ProfileViewModel
        function onLoadSuccess() {
            showSuccessView()
        }
    }

    Connections {
        target: ProfileViewModel
        function onLoadFailed(message) {
            showError("Error: " + message, 4000)
            showErrorView()
        }
    }

    Connections {
        target: ProfileViewModel
        function onUpdateProfileFailed(message) {
            showError("Error: " + message)
            btn_profile.loading = false
        }
    }
    Connections {
        target: ProfileViewModel
        function onUpdateProfileSuccess() {
            showSuccess("操作成功")
            btn_profile.loading = false
        }
    }

    Connections {
        target: ProfileViewModel
        function onUpdatePasswordFailed(message) {
            showError("Error: " + message)
            btn_passwd.loading = false
        }
    }
    Connections {
        target: ProfileViewModel
        function onUpdatePasswordSuccess() {
            showSuccess("操作成功")
            btn_passwd.loading = false
        }
    }
}

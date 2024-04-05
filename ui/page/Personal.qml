import QtQuick 2.15
import QtQuick.Layouts
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
                text: "Modify Profile"
                font.pointSize: 18
            }
            Column {
                anchors.centerIn: parent
                spacing: 20
                TextBox {
                    id: textBox_firstName
                    width: 300
                    hint: "first name"
                    text: ProfileViewModel.firstName
                }
                TextBox {
                    id: textBox_lastName
                    width: 300
                    hint: "last name"
                    text: ProfileViewModel.lastName
                }
                TextBox {
                    id: textBox_email
                    width: 300
                    hint: "email"
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
                text: "ok"
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
                text: "Modify Password"
                font.pointSize: 18
            }
            Column {
                anchors.centerIn: parent
                spacing: 20
                TextBox {
                    width: 300
                    hint: "old password"
                }
                TextBox {
                    id: textBox_passwd
                    width: 300
                    hint: "new password"
                }
                TextBox {
                    id: textBox_passwdRetype
                    width: 300
                    hint: "confirm password"
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
                text: "ok"
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

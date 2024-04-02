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
                    width: 300
                    hint: "first name"
                }
                TextBox {
                    width: 300
                    hint: "last name"
                }
                TextBox {
                    id: textBox_email
                    width: 300
                    hint: "email"
                }
            }
            RaisedButton {
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
                    width: 300
                    hint: "new password"
                }
                TextBox {
                    id: textBox_passwd
                    width: 300
                    hint: "confirm password"
                }
            }
            RaisedButton {
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
            }
        }
    }
}

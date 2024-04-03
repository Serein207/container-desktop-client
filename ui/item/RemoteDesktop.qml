import QtQuick 2.15
import QtQuick.Controls
import container_desktop
import "../component"

Item {
    property string vmId
    Row {
        spacing: 10
        TextBox {
            id: textBox
            hint: "连接密码"
            width: 200
            font.pointSize: 12
            echoMode: TextField.Password
            anchors.verticalCenter: parent.verticalCenter
        }
        FlatButton {
            text: "连接"
            hasBorder: true
            font.pointSize: 12
            textColor: "#2196f3"
            borderColor: "#2196f3"
            height: 30
            anchors.verticalCenter: parent.verticalCenter
            //enabled: textBox.text !== ""
            onClicked: {
                ContainerViewModel.connectVnc(vmId)
            }
        }
    }
}

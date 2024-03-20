import QtQuick 2.15
import "../component"

Area {
    id: control
    property url source: "qrc:/res/img/kylinOS.png"
    property string name: ""
    property string info: ""
    property string createTime: ""
    property string status: ""
    property string leftMemory: ""
    property string remark: ""
    signal deleteClicked
    signal rebootClicked
    signal settingClicked
    width: 366
    height: 236
    radius: 12
    Image {
        id: img
        width: 46
        height: 46
        source: control.source
        fillMode: Image.PreserveAspectFit
        anchors {
            top: parent.top
            left: parent.left
            leftMargin: 20
            topMargin: 20
        }
    }
    ToggleButton {
        anchors {
            right: parent.right
            top: parent.top
            topMargin: 20
            rightMargin: 20
        }
    }
    Text {
        id: name
        text: control.name
        font.pointSize: 16
        anchors {
            top: img.top
            left: img.right
            leftMargin: 10
        }
    }
    Text {
        id: info
        text: control.info
        font.pointSize: 12
        color: "#718096"
        anchors {
            bottom: img.bottom
            left: img.right
            leftMargin: 10
        }
    }
    Text {
        id: text1
        font.pointSize: 12
        color: "#718096"
        text: String("创建于：%1  状态：%2  剩余：%3").arg(control.createTime).arg(
                  control.status).arg(control.leftMemory)
        anchors {
            top: img.bottom
            left: parent.left
            leftMargin: 15
            topMargin: 20
        }
    }
    Text {
        id: text2
        font.pointSize: 12
        color: "#718096"
        text: String("备注：%1").arg(control.remark)
        anchors {
            top: text1.bottom
            left: text1.left
            topMargin: 15
        }
    }
    Row {
        anchors {
            bottom: parent.bottom
            bottomMargin: 20
            horizontalCenter: parent.horizontalCenter
        }
        spacing: 25
        FlatButton {
            text: "删除"
            textColor: "#FF5733"
            hasBorder: true
            borderColor: textColor
            radius: 8
            height: 48
            onClicked: {
                control.deleteClicked()
            }
        }
        FlatButton {
            text: "重启"
            textColor: "#00BAAD"
            hasBorder: true
            borderColor: textColor
            radius: 8
            height: 48
            onClicked: {
                control.rebootClicked()
            }
        }
        FlatButton {
            text: "设置"
            textColor: "#808080"
            hasBorder: true
            borderColor: textColor
            radius: 8
            height: 48
            onClicked: {
                control.settingClicked()
            }
        }
    }
}

import QtQuick 2.15
import QtQuick.Controls
import container_desktop
import "../component"

Item {
    Text {
        anchors {
            top: parent.top
            topMargin: 10
        }
        text: "容器信息"
        font.pointSize: 16
    }
    Column {
        id: col_header
        anchors {
            top: parent.top
            topMargin: 50
        }
        spacing: 5
        Repeater {
            Text {
                text: modelData
                font.pointSize: 14
                font.bold: Font.Bold
            }
            model: ["主机名", "系统类型", "内存", "数据卷", "架构", "核心数"]
        }
    }
    Column {
        anchors {
            top: col_header.top
            left: col_header.right
            leftMargin: 20
        }
        spacing: 5
        Repeater {
            Row {
                Text {
                    text: modelData
                    font.pointSize: 14
                }
            }
            model: [ConfigViewModel.hostname, ConfigViewModel.ostype, ConfigViewModel.memory, ConfigViewModel.rootfs, ConfigViewModel.arch, ConfigViewModel.cores]
        }
    }

    FlatButton {
        anchors {
            top: col_header.bottom
            topMargin: 20
        }
        hasBorder: true
        borderColor: "#ff8383"
        text: "删除容器"
        textColor: "#ff8383"
    }
}

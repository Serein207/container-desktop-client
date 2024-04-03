import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls
import container_desktop
import "../component"

Item {
    id: control
    property string vmId
    Item {
        width: parent.width
        height: 50
        Text {
            text: "名称"
            font.pointSize: 14
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 30
            }
        }
        Text {
            text: "描述"
            font.pointSize: 14
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 150
            }
        }
        IconButton {
            radius: 5
            display: Button.TextBesideIcon
            height: 40
            iconUrl: "qrc:/res/img/plus.svg"
            iconColor: "#666"
            text: "创建快照"
            textColor: "#666"
            normalColor: "#cdecff"
            hoverColor: "#9bdcff"
            border.width: 1
            border.color: "#73b7ff"
            anchors {
                right: parent.right
                rightMargin: 30
                verticalCenter: parent.verticalCenter
            }
        }

        Rectangle {
            width: parent.width - 40
            height: 2
            color: "#999"
            anchors {
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }
        }
    }
    Flickable {
        id: flickview
        clip: true
        anchors.fill: parent
        anchors.topMargin: 50
        contentWidth: parent.width
        contentHeight: container.implicitHeight
        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AsNeeded
            anchors {
                right: flickview.right
            }
        }
        boundsBehavior: Flickable.StopAtBounds
        Column {
            id: container
            anchors.fill: parent
            Repeater {
                delegate: snapshot
                model: 10
            }
        }
    }

    Component {
        id: snapshot
        Item {
            width: control.width
            height: 50
            Text {
                text: "current"
                font.pointSize: 12
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    leftMargin: 30
                }
            }
            Text {
                text: "You are here!"
                font.pointSize: 12
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    leftMargin: 150
                }
            }
            Row {
                anchors {
                    right: parent.right
                    rightMargin: 30
                    verticalCenter: parent.verticalCenter
                }
                spacing: 10
                RaisedButton {
                    text: "回滚"
                    font.pointSize: 12
                    font.weight: Font.Normal
                    height: 30
                    width: 50
                }
                RaisedButton {
                    text: "删除"
                    color: "#ff8383"
                    font.pointSize: 12
                    font.weight: Font.Normal
                    height: 30
                    width: 50
                }
            }
            Rectangle {
                width: control.width - 40
                height: 2
                color: "#999"
                anchors {
                    bottom: parent.bottom
                    horizontalCenter: parent.horizontalCenter
                }
            }
        }
    }
}

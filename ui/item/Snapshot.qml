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
            onClicked: {
                dialog1.open()
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

    Dialog {
        id: dialog1
        width: 170
        height: 150
        x: (parent.width - width) / 2 - 65
        Column {
            anchors.horizontalCenter: parent.horizontalPadding
            spacing: 10
            Text {
                text: "输入快照名称"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            TextBox {
                id: textBox
                width: 150
                anchors.horizontalCenter: parent.horizontalCenter
                font.pointSize: 12
            }
            Row {
                spacing: 5
                RaisedButton {
                    text: "确认"
                    color: "#2196f3"
                    textColor: "#fff"
                    rippleColor: "#fff"
                    font.pointSize: 12
                    width: 70
                    height: 30
                    enabled: textBox.text !== ""
                    onClicked: {
                        SnapshotViewModel.createSnapshot(vmId, textBox.text)
                        dialog1.close()
                    }
                }
                RaisedButton {
                    text: "取消"
                    font.pointSize: 12
                    width: 70
                    height: 30
                    onClicked: {
                        dialog1.close()
                    }
                }
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
                model: SnapshotViewModel
            }
        }
    }

    Component {
        id: snapshot
        Item {
            width: control.width
            height: 50
            Text {
                text: model.name
                font.pointSize: 12
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    leftMargin: 30
                }
            }
            Text {
                text: model.description
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
                    onClicked: {
                        SnapshotViewModel.rollbackSnapshot(vmId, model.name)
                    }
                }
                RaisedButton {
                    text: "删除"
                    color: "#ff8383"
                    font.pointSize: 12
                    font.weight: Font.Normal
                    height: 30
                    width: 50
                    onClicked: {
                        dialog.open()
                    }
                }
                Dialog {
                    id: dialog
                    height: 100
                    width: 170
                    Column {
                        spacing: 10
                        Text {
                            text: "确认删除？"
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                        Row {
                            spacing: 5
                            RaisedButton {
                                text: "确认"
                                color: "#2196f3"
                                textColor: "#fff"
                                rippleColor: "#fff"
                                font.pointSize: 12
                                width: 70
                                height: 30
                                onClicked: {
                                    SnapshotViewModel.deleteSnapshot(vmId,
                                                                     model.name)
                                    dialog.close()
                                }
                            }
                            RaisedButton {
                                text: "取消"
                                font.pointSize: 12
                                width: 70
                                height: 30
                                onClicked: {
                                    dialog.close()
                                }
                            }
                        }
                    }
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

    Connections {
        target: SnapshotViewModel
        function onLoadSuccess() {
            showSuccess("操作成功", 4000)
        }
    }

    Connections {
        target: SnapshotViewModel
        function onLoadFailed(message) {
            showError("Error: " + message, 4000)
        }
    }
}

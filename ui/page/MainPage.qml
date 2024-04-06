import QtQuick 2.15
import QtQuick.Controls
import container_desktop
import "../component"
import "../page"

Item {
    id: control

    SwipeView {
        id: swipeView
        currentIndex: navigation.currentIndex
        anchors.fill: parent
        Manager {
            id: manager
        }
        Panel {
            id: panel
        }
        Personal {
            id: personal
        }
        onCurrentIndexChanged: {
            if (currentIndex === 0) {
                manager.showLoadingView()
                ContainerBlockViewModel.load()
            }
            if (currentIndex === 1) {
                panel.showLoadingView()
                PanelViewModel.load(ContainerBlockViewModel.getFirstVmId(),
                                    "hour")
            }
            if (currentIndex === 2) {
                manager.showLoadingView()
                ProfileViewModel.load()
            }
        }
    }

    Navigation {
        id: navigation
        width: control.width
        height: 60
        target: swipeView
        blurRadius: 60
        model: ListModel {
            ListElement {
                name: "容器管理"
                iconUrl: "qrc:/res/img/manager.svg"
            }
            ListElement {
                name: "运维面板"
                iconUrl: "qrc:/res/img/panel.svg"
            }
            ListElement {
                name: "个人中心"
                iconUrl: "qrc:/res/img/personal.svg"
            }
        }
        SearchBox {
            anchors {
                left: parent.left
                leftMargin: 50
                verticalCenter: parent.verticalCenter
            }
        }
        FlatButton {
            text: LoginViewModel.username
            upper: false
            anchors {
                right: parent.right
                rightMargin: 40
                verticalCenter: parent.verticalCenter
            }
        }
    }

    ContentDialog {
        id: dialog
        title: "创建容器"
        width: 1000
        height: 540
        x: (parent.width - dialog.width) / 2
        y: 120
        Item {
            anchors {
                top: parent.top
                topMargin: 55
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
                bottomMargin: 20
            }
            width: 780
            GridView {
                model: VzTemplateViewModel
                cellHeight: 390
                cellWidth: 260
                topMargin: 10
                clip: true
                implicitHeight: contentHeight
                anchors.fill: parent
                delegate: Area {
                    width: 240
                    height: 380
                    Image {
                        id: img
                        source: model.url
                        width: 150
                        height: 150
                        fillMode: Image.PreserveAspectFit
                        anchors {
                            top: parent.top
                            topMargin: 10
                            horizontalCenter: parent.horizontalCenter
                        }
                    }
                    Text {
                        id: text
                        text: model.description
                        anchors {
                            left: parent.left
                            right: parent.right
                            leftMargin: 15
                            rightMargin: 15
                            top: img.bottom
                            topMargin: 15
                        }
                        wrapMode: Text.WordWrap
                        font.pointSize: 12
                        color: "#718096"
                    }
                    FlatButton {
                        id: btn
                        text: "创建"
                        textColor: "#00BAAD"
                        hasBorder: true
                        borderColor: "#00BAAD"
                        anchors {
                            horizontalCenter: parent.horizontalCenter
                            bottom: parent.bottom
                            bottomMargin: 15
                        }
                        font.pointSize: 12
                        onClicked: {
                            if (model.name === "ubuntuKylin")
                                dialog1.open()
                            else
                                showWarning("暂不支持该系统 :(", 3000)
                        }
                    }
                    Dialog {
                        id: dialog1
                        height: 320
                        width: 430
                        x: (dialog.width - width) / 2 - 65
                        Column {
                            spacing: 10
                            Text {
                                text: "容器创建设置——" + model.name
                                font.pointSize: 16
                            }
                            Grid {
                                columns: 2
                                columnSpacing: 10
                                rows: 3
                                rowSpacing: 15
                                TextBox {
                                    id: userPasswd
                                    hint: "系统密码"
                                    width: 200
                                    font.pointSize: 12
                                    echoMode: TextField.Password
                                }
                                TextBox {
                                    id: userPasswd2
                                    hint: "确认系统密码"
                                    width: 200
                                    font.pointSize: 12
                                    echoMode: TextField.Password
                                }
                                TextBox {
                                    id: vncPasswd
                                    hint: "桌面密码"
                                    width: 200
                                    font.pointSize: 12
                                    echoMode: TextField.Password
                                }
                                TextBox {
                                    id: vncPasswd2
                                    hint: "确认桌面密码"
                                    width: 200
                                    font.pointSize: 12
                                    echoMode: TextField.Password
                                }
                                TextBox {
                                    id: containerName
                                    hint: "容器名称"
                                    width: 200
                                    font.pointSize: 12
                                }
                                TextBox {
                                    id: remark
                                    hint: "系统用途备注"
                                    width: 200
                                    font.pointSize: 12
                                }
                            }
                            Row {
                                spacing: 5
                                RaisedButton {
                                    text: "确认"
                                    color: "#2196f3"
                                    textColor: "#fff"
                                    rippleColor: "#fff"
                                    font.pointSize: 12
                                    width: 200
                                    height: 30
                                    enabled: userPasswd.text !== ""
                                             && userPasswd.text === userPasswd2.text
                                             && vncPasswd.text !== ""
                                             && vncPasswd.text === vncPasswd2.text
                                             && containerName.text !== ""
                                    onClicked: {
                                        VzTemplateViewModel.createContainer(
                                                    model.osTemplate,
                                                    userPasswd.text,
                                                    vncPasswd.text)
                                        dialog1.close()
                                    }
                                }
                                RaisedButton {
                                    text: "取消"
                                    font.pointSize: 12
                                    width: 200
                                    height: 30
                                    onClicked: {
                                        dialog1.close()
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Connections {
        target: VzTemplateViewModel
        function onLoadFailed(message) {
            showError("Error: " + message, 4000)
        }
    }

    FloatingActionButton {
        anchors {
            bottom: parent.bottom
            right: parent.right
            bottomMargin: 30
            rightMargin: 25
        }
        iconSource: "qrc:/res/img/download.png"
        onClicked: {
            VzTemplateViewModel.load()
            dialog.open()
        }
    }
}

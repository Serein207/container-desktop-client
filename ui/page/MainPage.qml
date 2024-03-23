import QtQuick 2.15
import QtQuick.Controls
import container_desktop
import "../component"

Item {
    id: control

    SwipeView {
        id: swipeView
        currentIndex: navigation.currentIndex
        anchors.fill: parent

        Manager {}
        Panel {}
        Personal {}
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

    FloatingActionButton {
        anchors {
            bottom: parent.bottom
            right: parent.right
            bottomMargin: 30
            rightMargin: 25
        }
        iconSource: "qrc:/res/img/download.png"
    }
}

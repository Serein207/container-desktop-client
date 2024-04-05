import QtQuick 2.15
import QtQuick.Controls

Popup {
    id: popup

    property alias dialogWidth: dialog.width
    property alias dialogHeight: dialog.height
    modal: true

    Item {
        id: dialog
        anchors.centerIn: parent
        Rectangle {
            id: background
            anchors.fill: parent
            color: "#fafafa"
            visible: false
            Item {
                anchors.fill: parent
            }
        }

        MouseArea {
            id: eventEater
            anchors.fill: parent
            enabled: (popup.state == "visible")
        }

        states: [
            State {
                name: "visible"
                PropertyChanges {
                    target: dialog
                    opacity: 1
                }
            },
            State {
                name: "hidden"
                PropertyChanges {
                    target: dialog
                    opacity: 0
                }
            }
        ]

        transitions: [
            Transition {
                NumberAnimation {
                    target: dialog
                    property: "opacity"
                    duration: 200
                    easing.type: Easing.Bezier
                    easing.bezierCurve: [0.4, 0, 0.2, 1, 1, 1]
                }
            },
            Transition {
                from: "hidden"
                to: "visible"
                NumberAnimation {
                    target: dialog
                    property: "scale"
                    from: 0.95
                    to: 1
                    duration: 200
                    easing.type: Easing.OutCubic
                }
            }
        ]
    }
}

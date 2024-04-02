import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window

Popup {
    id: popup
    padding: 0
    modal: true
    anchors.centerIn: Overlay.overlay
    closePolicy: Popup.CloseOnEscape
    enter: Transition {
        NumberAnimation {
            property: "opacity"
            duration: 83
            from: 0
            to: 1
        }
    }
    exit: Transition {
        NumberAnimation {
            property: "opacity"
            duration: 83
            from: 1
            to: 0
        }
    }
    background: Rectangle {
        radius: 5
        color: Qt.rgba(1, 1, 1, 1)
        Shadow {
            radius: 5
        }
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

TextField {
    signal commit(string text)
    property bool disabled: false
    property color placeholderNormalColor: "#767676"
    property color placeholderFocusColor: "#335eea"
    id: control
    padding: 7
    width: 200
    height: 30
    leftPadding: padding + 28
    rightPadding: 20
    enabled: !disabled
    placeholderText: "Search"
    font.pixelSize: 16
    color: activeFocus ? "#335eea" : "#767676"
    selectedTextColor: control.color
    placeholderTextColor: {
        if (activeFocus) {
            return placeholderFocusColor
        }
        return placeholderNormalColor
    }
    selectByMouse: true
    background: Rectangle {
        property Item inputItem
        inputItem: control
        radius: 6
        Rectangle {
            radius: 6
            anchors.fill: parent
            color: control.activeFocus ? "#eaeffd" : "#f2f2f4"
        }
    }
    Keys.onEnterPressed: event => d.handleCommit(event)
    Keys.onReturnPressed: event => d.handleCommit(event)
    QtObject {
        id: d
        function handleCommit(event) {
            control.commit(control.text)
        }
    }

    Item {
        width: 20
        height: 20
        Layout.alignment: Qt.AlignCenter
        Image {
            id: img
            anchors.fill: parent
            sourceSize.width: 30
            sourceSize.height: 30
            fillMode: Image.PreserveAspectFit
            source: "qrc:/res/img/search.svg"
        }
        ColorOverlay {
            anchors.fill: parent
            source: img
            color: control.activeFocus ? "#335eea" : "#767676"
        }
        anchors {
            left: parent.left
            leftMargin: 10
            verticalCenter: parent.verticalCenter
        }
    }
    IconButton {
        anchors {
            right: parent.right
            rightMargin: 5
        }
        iconUrl: "qrc:/res/img/cancel.svg"
        iconSize: 12
        Layout.preferredWidth: 30
        Layout.preferredHeight: 20
        Layout.alignment: Qt.AlignVCenter
        iconColor: control.activeFocus ? "#335eea" : "#767676"
        visible: item_mouse.containsMouse && control.text !== ""
        onClicked: {
            control.text = ""
        }
    }
    MouseArea {
        id: item_mouse
        anchors.fill: parent
        cursorShape: Qt.IBeamCursor
        acceptedButtons: Qt.RightButton
        hoverEnabled: true
    }
}

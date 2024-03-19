import QtQuick 2.15

Item {
    id: button
    width: 56 * 1
    height: 56 * 1

    property alias color: background.color
    property alias rippleColor: ripple.color
    property alias iconSource: icon.source

    signal clicked

    Rectangle {
        id: background
        anchors.fill: parent
        radius: 28 * 1
        visible: false
    }

    PaperShadow {
        id: shadow
        source: background
        depth: button.enabled ? (mouseArea.pressed ? 2 : 3) : 0
    }

    PaperRipple {
        id: ripple
        radius: 28 * 1
        color: "#deffffff"
        mouseArea: mouseArea
    }

    Image {
        id: icon
        anchors.centerIn: parent
        width: 24 * 1
        height: 24 * 1
        sourceSize.width: width
        sourceSize.height: height
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        enabled: button.enabled
        onClicked: button.clicked()
    }
}

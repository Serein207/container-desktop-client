import QtQuick 2.15
import "."

Item {
    id: button
    width: (inline
            || label.paintedWidth > 56 * 1) ? (label.paintedWidth + 32 * 1) : (88 * 1)
    height: 36 * 1

    property bool inline: false
    property alias text: label.text
    property alias textColor: label.color
    property alias fontSize: label.font.pointSize
    property bool upper: true
    property bool hasBorder: fasle
    property color borderColor: Qt.rgba(226 / 255, 229 / 255, 234 / 255, 1)
    property alias radius: background.radius

    signal clicked

    Rectangle {
        id: background
        anchors.fill: parent
        radius: 3 * 1
        color: button.enabled ? "#00999999" : "#1a999999"
        border.width: hasBorder ? 1 : 0
        border.color: borderColor
    }

    Text {
        id: label
        anchors.centerIn: parent
        font.family: UIConstants.sansFontFamily
        font.pointSize: UIConstants.bodyFontSize
        font.weight: Font.DemiBold
        font.capitalization: upper ? Font.AllUppercase : Font.Normal
        color: button.enabled ? UIConstants.bodyTextColor : UIConstants.displayTextColor
        opacity: button.enabled ? 1 : 0.62

        Behavior on opacity {
            NumberAnimation {
                duration: 200
                easing.type: Easing.Bezier
                easing.bezierCurve: [0.4, 0, 0.2, 1, 1, 1]
            }
        }
    }

    PaperRipple {
        id: ripple
        radius: 3 * 1
        mouseArea: mouseArea
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        enabled: button.enabled
        onClicked: button.clicked()
    }
}

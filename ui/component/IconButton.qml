import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

Item {
    id: control
    width: btn.implicitWidth
    height: btn.implicitHeight
    property int radius: 10
    property var iconUrl
    property int iconWidth: 16
    property int iconHeight: 16
    property color hoverColor: Qt.rgba(209 / 255, 209 / 255, 214 / 255, 0.28)
    property alias normalColor: btn.normalColor
    property color textColor
    property color iconColor
    property int iconSize
    property alias hovered: btn.hovered
    property int duration: 2
    signal clicked
    signal doubleClicked
    property alias background: btn.background
    property int display: Button.IconOnly
    property alias text: btn.text
    property alias border: bgd.border
    Button {
        id: btn
        property color normalColor: "transparent"
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        display: control.display
        background: Rectangle {
            id: bgd
            implicitWidth: 30
            implicitHeight: 30
            radius: control.radius
            color: hovered ? hoverColor : normalColor
        }
        contentItem: RowLayout {
            Item {
                width: iconWidth
                height: iconHeight
                Layout.alignment: Qt.AlignCenter
                Image {
                    id: img
                    anchors.fill: parent
                    sourceSize.width: iconWidth
                    sourceSize.height: iconHeight
                    fillMode: Image.PreserveAspectFit
                    source: iconUrl
                }
                ColorOverlay {
                    anchors.fill: parent
                    source: img
                    color: control.iconColor
                }
            }
            Text {
                text: control.text
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                visible: display !== Button.IconOnly
                color: control.textColor
                font.pixelSize: 18
            }
        }

        Behavior on width {
            NumberAnimation {
                duration: control.duration
            }
        }
        Behavior on height {
            NumberAnimation {
                duration: control.duration
            }
        }
        onPressed: {
            width = width * 0.95
            height = height * 0.95
        }
        onReleased: {
            width = width / 0.95
            height = height / 0.95
        }
        onClicked: {
            control.clicked()
        }
        onDoubleClicked: {
            control.doubleClicked()
        }
    }
}

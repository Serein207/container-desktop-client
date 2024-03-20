import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls

Button {
    id: control
    property color backgroundColor: "#f5f5f7"
    property color activeColor: "#335eea"
    property bool disabled: false
    property string contentDescription: ""
    property color disableColor: Qt.rgba(233 / 255, 233 / 255, 233 / 255, 1)
    property color checkColor: activeColor
    property color normalColor: backgroundColor
    property real textSpacing: 6
    property bool textRight: true
    property var clickListener: function () {
        checked = !checked
    }
    background: Item {
        implicitHeight: 32
        implicitWidth: 52
    }
    Accessible.role: Accessible.Button
    Accessible.name: control.text
    Accessible.description: contentDescription
    Accessible.onPressAction: control.clicked()
    enabled: !disabled
    focusPolicy: Qt.TabFocus
    onClicked: clickListener()
    padding: 0
    horizontalPadding: 0
    onCheckableChanged: {
        if (checkable) {
            checkable = false
        }
    }
    contentItem: RowLayout {
        spacing: control.textSpacing
        layoutDirection: control.textRight ? Qt.LeftToRight : Qt.RightToLeft
        Rectangle {
            id: control_backgound
            width: background.width
            height: background.height
            radius: 10
            color: disabled ? disableColor : (checked ? checkColor : normalColor)
            border.width: 1
            border.color: Qt.rgba(226 / 255, 229 / 255, 234 / 255, 1)
            Rectangle {
                width: 20
                height: 20
                radius: 5
                x: checked ? control_backgound.width - width - 6 : 6
                y: (control_backgound.height - height) / 2
                scale: {
                    if (pressed) {
                        return 9 / 10
                    }
                    return hovered ? 12 / 10 : 1
                }
                color: "#fff"
                Behavior on scale {
                    NumberAnimation {
                        duration: 167
                        easing.type: Easing.OutCubic
                    }
                }
                Behavior on x {
                    NumberAnimation {
                        duration: 167
                        easing.type: Easing.OutCubic
                    }
                }
                Shadow {
                    radius: 5
                    visible: !checked
                }
            }
        }
    }
}

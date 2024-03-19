import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls

BlurRectangle {
    id: control
    property var model
    property int currentIndex: 0
    readonly property color activeColor: "#335eea"
    Row {
        anchors.centerIn: parent
        spacing: 50
        Repeater {
            id: rep
            model: control.model
            delegate: IconButton {
                display: Button.TextBesideIcon
                text: model.name
                iconUrl: model.iconUrl
                textColor: currentIndex == index ? activeColor : "#000"
                iconColor: currentIndex == index ? activeColor : "#000"
                onClicked: {
                    currentIndex = index
                }
            }
        }
    }
}

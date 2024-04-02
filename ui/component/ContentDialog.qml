import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window

Popup {
    id: control
    property string title: ""
    property string message: ""
    property int delayTime: 100
    property var contentDelegate: Component {
        Item {}
    }
    property var onNeutralClickListener
    property var onNegativeClickListener
    property var onPositiveClickListener
    signal neutralClicked
    signal negativeClicked
    signal positiveClicked
    implicitWidth: 800
    implicitHeight: layout_content.height + 80
    focus: true
    Rectangle {
        id: layout_content
        width: parent.width
        height: layout_column.childrenRect.height
        color: 'transparent'
        radius: 5
        IconButton {
            iconUrl: "qrc:/res/img/cancel.svg"
            anchors {
                top: parent.top
                right: parent.right
                topMargin: 10
                rightMargin: 10
            }
            onClicked: {
                control.close()
            }
        }
        ColumnLayout {
            id: layout_column
            width: parent.width
            Text {
                id: text_title
                font.pointSize: 16
                text: title
                topPadding: 20
                leftPadding: 20
                rightPadding: 20
                wrapMode: Text.WrapAnywhere
            }
            Loader {
                sourceComponent: control.contentDelegate
                Layout.fillWidth: true
                Layout.preferredHeight: status === Loader.Ready ? item.height : 0
            }
        }
    }
}
